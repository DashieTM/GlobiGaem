// Fill out your copyright notice in the Description page of Project Settings.


#include "characterthatworks.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TimerManager.h"
#include "Projectile.h"
#include "MenuHUD.h"
#include "SoccerGameMode.h"
#include "Net/UnrealNetwork.h"
#include "SoccerPlayerController.h"
#include "GameFramework/Character.h" 
#include "PowerUp.h"
#include "BobbyPlayerState.h"



// Sets default values
Acharacterthatworks::Acharacterthatworks()
{
	

	

	SetReplicateMovement(true);
	SetReplicates(true);
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	PowerUpStrenght = 1500.f;
	bHasPowerUp = true;
	bCanDash = true;
	DashCD = 2.f;
	DashLenght = 8000.f;
	DashFriction = 0.5f;
	DashFrictionAir = 38.0f;
	PowerUpCD = 25.f;
	BobbyNameText.FromString("");
	

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->AirControl = 0.8f;
	GetCharacterMovement()->JumpZVelocity = 600.0f;
	GetCharacterMovement()->MaxAcceleration = 10000.f;
	GetCharacterMovement()->MaxWalkSpeed = 1100.0f;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	SoundEffectBobby = CreateDefaultSubobject<UAudioComponent>("Sound");
	SoundEffectBobby->SetupAttachment(FollowCamera);
	ShootingPosition = 400.f;
	BobbyName = CreateDefaultSubobject<UTextRenderComponent>(TEXT("BobbyName"));
	BobbyName->SetupAttachment(RootComponent);
	BobbyName->SetText("");
	
	

	FollowCamera->bUsePawnControlRotation = false;
	bCanFire = true;

}


void Acharacterthatworks::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(Acharacterthatworks,BobbyBuffer);
	DOREPLIFETIME(Acharacterthatworks, BobbyBufferOld);
	DOREPLIFETIME(Acharacterthatworks, BobbyNameText);
	

}


// Called when the game starts or when spawned
void Acharacterthatworks::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void Acharacterthatworks::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// Called to bind functionality to input
void Acharacterthatworks::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAxis("MoveForward", this, &Acharacterthatworks::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &Acharacterthatworks::MoveRight);
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &Acharacterthatworks::Shoot);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &Acharacterthatworks::Dash);
	PlayerInputComponent->BindAction("UsePowerUp", IE_Pressed, this, &Acharacterthatworks::UsePowerUp);
	
}



void Acharacterthatworks::MoveForward(float Axis)
{
	FRotator Rotation = Controller->GetControlRotation();
	FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
	FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, Axis);
}

void Acharacterthatworks::MoveRight(float Axis)
{
	FRotator Rotation = Controller->GetControlRotation();
	FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
	FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, Axis);
}



void Acharacterthatworks::ResetDash()
{ 
	bCanDash = true;
	GetCharacterMovement()->FallingLateralFriction = 8.f;
	GetCharacterMovement()->BrakingFrictionFactor = 2.f;
	GetWorldTimerManager().ClearTimer(MemberTimerHandle2);
	ServerResetDash();
}


void Acharacterthatworks::ServerResetDash_Implementation()
{
	GetCharacterMovement()->FallingLateralFriction = 8.f;
	GetCharacterMovement()->BrakingFrictionFactor = 2.f;
}

void Acharacterthatworks::Dash()
{
	if (bCanDash)
	{
		if(GetLocalRole() < ROLE_Authority)
		{
			bCanDash = false;
			GetCharacterMovement()->bIgnoreClientMovementErrorChecksAndCorrection = true;
			ServerDash();
			GetWorld()->GetTimerManager().SetTimer(MemberTimerHandle2, this, &Acharacterthatworks::ResetDash, DashCD, false);
			GetCharacterMovement()->bIgnoreClientMovementErrorChecksAndCorrection = false;
			return;
		}
	
	bCanDash = false;
	GetCharacterMovement()->BrakingFrictionFactor = DashFriction;
	GetCharacterMovement()->FallingLateralFriction = DashFrictionAir;
	LaunchCharacter(FVector(FollowCamera->GetForwardVector().X, FollowCamera->GetForwardVector().Y, 0).GetSafeNormal() * DashLenght, true, true);
	GetWorld()->GetTimerManager().SetTimer(MemberTimerHandle2, this, &Acharacterthatworks::ResetDash, DashCD, false);
	}

	
	
}

void Acharacterthatworks::ServerDash_Implementation()
{
	ClientDash();
	GetCharacterMovement()->BrakingFrictionFactor = DashFriction;
	GetCharacterMovement()->FallingLateralFriction = DashFrictionAir;
	LaunchCharacter(FVector(FollowCamera->GetForwardVector().X, FollowCamera->GetForwardVector().Y, 0).GetSafeNormal() * DashLenght, true, true);
	
}

void Acharacterthatworks::ClientDash_Implementation()
{
	GetCharacterMovement()->BrakingFrictionFactor = DashFriction;
	GetCharacterMovement()->FallingLateralFriction = DashFrictionAir;
	LaunchCharacter(FVector(FollowCamera->GetForwardVector().X, FollowCamera->GetForwardVector().Y, 0).GetSafeNormal() * DashLenght, true, true);
}

void Acharacterthatworks::ServerShoot_Implementation()
{
	Shoot();
}


void Acharacterthatworks::Shoot()
{
	if(bCanFire) //check fire status >> player allowed to fire?
	{ 
		
		if(GetLocalRole() < ROLE_Authority)
		{
			ServerShoot();
			bCanFire = false;
			SoundEffectBobby->Play();
			GetWorld()->GetTimerManager().SetTimer(MemberTimerHandle, this, &Acharacterthatworks::ResetShoot, 2.5f, false);
			return;
		}

		FTransform SpawnTransform = FollowCamera->GetComponentTransform();
		SpawnTransform.SetLocation(FollowCamera->GetComponentRotation().Vector() * ShootingPosition + FollowCamera->GetComponentLocation());
		FActorSpawnParameters SpawnParams;
		GetWorld()->SpawnActor<AProjectile>(ProjectileBP, SpawnTransform, SpawnParams);
		SoundEffectBobby->Play();
		GetWorld()->GetTimerManager().SetTimer(MemberTimerHandle, this, &Acharacterthatworks::ResetShoot, 2.5f, false);
		//set fire to false after player has fired.
		bCanFire = false;
	}
}

void Acharacterthatworks::ResetShoot()
{
	bCanFire = true;
	GetWorldTimerManager().ClearTimer(MemberTimerHandle);

}

UWorld* Acharacterthatworks::OpenLevel()
{
	return GetWorld();
}

void Acharacterthatworks::OnRep_CurrentName()
{
	UpdateName();
}

void Acharacterthatworks::UpdateName()
{
	BobbyName->SetText(BobbyNameText);
}

FString Acharacterthatworks::ReturnFireStatus()
{
	if (bCanFire)
	{
		return "ready";
	}
	return "cooldown";
}

FString Acharacterthatworks::ReturnDashStatus()
{
	if (bCanDash)
	{
		return "ready";
	}
	return "cooldown";
}

FString Acharacterthatworks::ReturnPowerUpStatus()
{
	if (bHasPowerUp)
	{
		return "ready";
	}
	return "empty";
}

void Acharacterthatworks::DeleteBobby(Acharacterthatworks* Bobby)
{
	if (GetLocalRole() < ROLE_Authority)
	{
		ServerDeleteBobby(Bobby);
		return;
	}
	
	BobbyBufferOld->Destroy();
}

void Acharacterthatworks::ServerDeleteBobby_Implementation(Acharacterthatworks* Bobby)
{
	DeleteBobby(Bobby);
}



void Acharacterthatworks::ServerSpawnBobbyDefault_Implementation(APlayerController* TheNewController)
{
	SpawnBobbyDefault(TheNewController);
}

void Acharacterthatworks::SpawnBobbyDefault(APlayerController* TheNewController)
{
	if (GetLocalRole() < ROLE_Authority)
	{
		ServerSpawnBobbyDefault(TheNewController);
		return;
	}
	
	BobbyBufferOld = Cast<Acharacterthatworks>(TheNewController->GetCharacter());
	FActorSpawnParameters SpawnParams;
	FTransform BallRespawn;
	BallRespawn.TransformPosition(FVector());
	BallRespawn.SetRotation(FQuat(FRotator(0.f, 0.f, 0.0f)));
	BallRespawn.SetScale3D(FVector(1.0f, 1.0f, 1.0f));
	BallRespawn.SetLocation(FVector(-4675.f, 0.f, 641.f));
	BobbyDefault = GetWorld()->SpawnActor<Acharacterthatworks>(Bobby_Default, BallRespawn, SpawnParams);
	Acharacterthatworks::SetBobbyBuffer(BobbyDefault);
	BobbyDefault->SetOwner(TheNewController);
	TheNewController->Possess(BobbyDefault);
	BobbyBufferOld->Destroy();
	
}




void Acharacterthatworks::ServerSpawnBobbyRed_Implementation(APlayerController* TheNewController)
{
	SpawnBobbyRed(TheNewController);
	
}

void Acharacterthatworks::SpawnBobbyRed(APlayerController* TheNewController)
{
	if (GetLocalRole() < ROLE_Authority)
	{
		ServerSpawnBobbyRed(TheNewController);
		
		return;
	}
	
	BobbyBufferOld = Cast<Acharacterthatworks>(TheNewController->GetCharacter());
	FActorSpawnParameters SpawnParams;
	FTransform BallRespawn;
	BallRespawn.TransformPosition(FVector());
	BallRespawn.SetRotation(FQuat(FRotator(0.f, 90.f, 0.0f)));
	BallRespawn.SetScale3D(FVector(1.0f, 1.0f, 1.0f));
	BallRespawn.SetLocation(FVector(0.f, -4500.f, 380.f));
	BobbyRed = GetWorld()->SpawnActor<Acharacterthatworks>(Bobby_Red, BallRespawn, SpawnParams);
	Acharacterthatworks::SetBobbyBuffer(BobbyRed);
	BobbyRed->SetOwner(TheNewController);
	TheNewController->Possess(BobbyRed);
	BobbyBufferOld->Destroy();
	
}



void Acharacterthatworks::ServerSpawnBobbyGreen_Implementation(APlayerController* TheNewController)
{
	SpawnBobbyGreen(TheNewController);
}

void Acharacterthatworks::SpawnBobbyGreen(APlayerController* TheNewController)
{
	
		if (GetLocalRole() < ROLE_Authority)
		{
			
			ServerSpawnBobbyGreen(TheNewController);
			
			return;
		}

		BobbyBufferOld = Cast<Acharacterthatworks>(TheNewController->GetCharacter());
		FActorSpawnParameters SpawnParams;
		FTransform BallRespawn;
		BallRespawn.TransformPosition(FVector());
		BallRespawn.SetRotation(FQuat(FRotator(0.f, -90.f, 0.0f)));
		BallRespawn.SetScale3D(FVector(1.0f, 1.0f, 1.0f));
		BallRespawn.SetLocation(FVector(0.f, 4500.f, 380.f));
		BobbyGreen = GetWorld()->SpawnActor<Acharacterthatworks>(Bobby_Green, BallRespawn, SpawnParams);
		BobbyBuffer = BobbyGreen;
		BobbyGreen->SetOwner(TheNewController);
		TheNewController->Possess(BobbyGreen);
		BobbyBufferOld->Destroy();
		
	
}

void Acharacterthatworks::SetBobbyBuffer(Acharacterthatworks* Bobby)
{
	BobbyBuffer = Bobby;
}

Acharacterthatworks* Acharacterthatworks::GetBobbyBuffer()
{
	return BobbyBuffer;
}

void Acharacterthatworks::ClearBobbyBuffer()
{
	Acharacterthatworks::SetBobbyBuffer(nullptr);
}

void Acharacterthatworks::ClientWhichTeam_Implementation(ASoccerPlayerController* SController)
{
	if (GetWorld()->GetName() == "soccer4p")
	{
		BobbyBuffer = Cast<Acharacterthatworks>(SController->GetCharacter());
		int Team = SController->GetTeam();
		switch (Team)
		{
			case 0:
			{
				break;
			}
			case 1:
			{
				Acharacterthatworks::SpawnBobbyGreen(SController);

				break;
			}
			case 2:
			{
				Acharacterthatworks::SpawnBobbyRed(SController);

			}
		}
	}
}

void Acharacterthatworks::UsePowerUp()
{
	if(bHasPowerUp)
	{ 
		bHasPowerUp = false;
		if (GetLocalRole() < ROLE_Authority)
		{
			ServerUsePowerUp();
			return;
		}
		LaunchCharacter(FVector(0.f, 0.f, PowerUpStrenght), false, true);
	}
}

void Acharacterthatworks::ServerUsePowerUp_Implementation()
{
	ClientUsePowerUp();
	LaunchCharacter(FVector(0.f, 0.f, PowerUpStrenght), false, true);
}

void Acharacterthatworks::ClientUsePowerUp_Implementation()
{
	LaunchCharacter(FVector(0.f, 0.f, PowerUpStrenght), false, true);
}

void Acharacterthatworks::ClientResetPowerUp_Implementation()
{
	bHasPowerUp = true;
}

//Hit Detection
void Acharacterthatworks::PowerUpCollected(FTransform& Power)
{
	ClientResetPowerUp();
	SpawnTransformPowerUp = Power;
	
}

void Acharacterthatworks::ServerSpawnPowerUpDelay_Implementation()
{
	ServerSpawnPowerUp(SpawnTransformPowerUp);
}

void Acharacterthatworks::ServerSpawnPowerUp_Implementation(FTransform Power)
{
	FActorSpawnParameters SpawnParams;
	GetWorld()->SpawnActor<APowerUp>(PowerUpBP, Power, SpawnParams);
}

bool Acharacterthatworks::HasPowerUp()
{
	return bHasPowerUp;
}

void Acharacterthatworks::SetBobbyName(const FText& LeName, ASoccerPlayerController* BobbyController)
{
	if (GetLocalRole() < ROLE_Authority)
	{
		ServerSetBobbyName(LeName, BobbyController);
		return;
	}
	BobbyNameText = LeName;
}


void Acharacterthatworks::ServerSetBobbyName_Implementation(const FText& LeName,ASoccerPlayerController* BobbyController)
{
	BobbyNameText = LeName;
}

void Acharacterthatworks::MultiSetBobbyName_Implementation(const FText& LeName, ASoccerPlayerController* BobbyController)
{
	
}


