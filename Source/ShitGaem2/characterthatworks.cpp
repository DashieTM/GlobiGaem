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
	bReplicates = true;
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	PowerUpStrenght = 1500.f;
	bHasPowerUp = true;
	bCanDash = true;
	DashCD = 1.7f;
	DashLenght =8000.f;
	DashFriction = 0.0f;
	DashFrictionAir = 38.0f;
	PowerUpCD = 25.f;
	BobbyNameText.FromString("");
	
	
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->AirControl = 0.8f;
	GetCharacterMovement()->JumpZVelocity = 700.0f;
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
	ShootingPosition = 105.f;
	BobbyName = CreateDefaultSubobject<UTextRenderComponent>(TEXT("BobbyName"));
	BobbyName->SetupAttachment(RootComponent);
	BobbyName->SetText("");
	
	FollowCamera->bUsePawnControlRotation = false;
	bCanFire = true;
}

//replication
void Acharacterthatworks::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(Acharacterthatworks,BobbyBuffer);
	DOREPLIFETIME(Acharacterthatworks, BobbyBufferOld);
	DOREPLIFETIME(Acharacterthatworks, BobbyNameText);
	DOREPLIFETIME(Acharacterthatworks, bCanFire);
	DOREPLIFETIME(Acharacterthatworks, SoundEffectBobby);
}


// Called when the game starts or when spawned
void Acharacterthatworks::BeginPlay()
{
	Super::BeginPlay();
	if (GetLocalRole() != ROLE_Authority) 
	{
		this->DisableComponentsSimulatePhysics();
	}
	
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


//Move forward and backward
void Acharacterthatworks::MoveForward(float Axis)
{
	FRotator Rotation = Controller->GetControlRotation();
	FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
	FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, Axis);
}

//move right and left
void Acharacterthatworks::MoveRight(float Axis)
{
	FRotator Rotation = Controller->GetControlRotation();
	FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
	FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, Axis);
}


//reset the dash cd(called after dash cd)
void Acharacterthatworks::ResetDash()
{ 
	bCanDash = true;
	GetWorldTimerManager().ClearTimer(MemberTimerHandle2);
	ServerResetDash();
}

//reset Friction after dash, called by server because of authority
void Acharacterthatworks::ServerResetDash_Implementation()
{
	bCanDash = true;
}


//dash implementation, dash is towards view angle
void Acharacterthatworks::Dash()
{
	if (bCanDash)
	{
		if(GetLocalRole() < ROLE_Authority)
		{
			bCanDash = false;
			ServerDash();
			GetCharacterMovement()->AddImpulse(FVector(FollowCamera->GetForwardVector().X, FollowCamera->GetForwardVector().Y, FollowCamera->GetForwardVector().Z).GetSafeNormal() * DashLenght, true);
			GetWorld()->GetTimerManager().SetTimer(MemberTimerHandle2, this, &Acharacterthatworks::ResetDash, DashCD, false);
			
			return;
		}
		GetCharacterMovement()->BrakingFrictionFactor = DashFriction;
		GetCharacterMovement()->FallingLateralFriction = DashFrictionAir;
		GetCharacterMovement()->AddImpulse(FVector(FollowCamera->GetForwardVector().X, FollowCamera->GetForwardVector().Y, FollowCamera->GetForwardVector().Z).GetSafeNormal() * DashLenght, true);
		bCanDash = false;
		GetCharacterMovement()->BrakingFrictionFactor = 2.f;
		GetCharacterMovement()->FallingLateralFriction = 8.f;
		GetWorld()->GetTimerManager().SetTimer(MemberTimerHandle2, this, &Acharacterthatworks::ResetDash, DashCD, false);
	}
}

//dash implementation for server, in order to not falsly correct position
void Acharacterthatworks::ServerDash_Implementation()
{
	GetCharacterMovement()->BrakingFrictionFactor = DashFriction;
	GetCharacterMovement()->FallingLateralFriction = DashFrictionAir;
	
	GetCharacterMovement()->AddImpulse(FVector(FollowCamera->GetForwardVector().X, FollowCamera->GetForwardVector().Y, FollowCamera->GetForwardVector().Z).GetSafeNormal() * DashLenght, true);
	GetCharacterMovement()->BrakingFrictionFactor = 2.f;
	GetCharacterMovement()->FallingLateralFriction = 8.f;
}



//client dash implementation for smooth movement
void Acharacterthatworks::ClientDash_Implementation()
{
	
}

//servershoot implementation, authority
void Acharacterthatworks::ServerShoot_Implementation()
{
	Shoot();
}

//actual shoot fucntion
void Acharacterthatworks::Shoot()
{
	if(bCanFire) //check fire status >> player allowed to fire?
	{ 
		if(GetLocalRole() < ROLE_Authority)
		{
			ServerShoot();
			return;
		}
		FTransform SpawnTransform = FollowCamera->GetComponentTransform();
		
		SpawnTransform.SetLocation(FollowCamera->GetComponentLocation() + FollowCamera->GetComponentRotation().Vector() * ShootingPosition );
		FActorSpawnParameters SpawnParams;
		GetWorld()->SpawnActor<AProjectile>(ProjectileBP, SpawnTransform, SpawnParams);
		ClientBobbySound();
		GetWorld()->GetTimerManager().SetTimer(MemberTimerHandle, this, &Acharacterthatworks::ResetShoot, 2.5f, false);
		//set fire to false after player has fired.
		bCanFire = false;
	}
}

//play sound in client
void Acharacterthatworks::ClientBobbySound_Implementation()
{
	SoundEffectBobby->Play();
}


//reset the dash cd
void Acharacterthatworks::ResetShoot()
{
	bCanFire = true;
	GetWorldTimerManager().ClearTimer(MemberTimerHandle);
}

//return the world the character is currently in
UWorld* Acharacterthatworks::OpenLevel()
{
	return GetWorld();
}

//update name on rep
void Acharacterthatworks::OnRep_CurrentName()
{
	UpdateName();
}

//update name
void Acharacterthatworks::UpdateName()
{
	BobbyName->SetText(BobbyNameText);
}

//return fire status, ready or cooldown
FString Acharacterthatworks::ReturnFireStatus()
{
	if (bCanFire)
	{
		return "ready";
	}
	return "cooldown";
}

//return dash status, ready or cooldown
FString Acharacterthatworks::ReturnDashStatus()
{
	if (bCanDash)
	{
		return "ready";
	}
	return "cooldown";
}

//return power up status, ready, or empty
FString Acharacterthatworks::ReturnPowerUpStatus()
{
	if (bHasPowerUp)
	{
		return "ready";
	}
	return "empty";
}

//delete the current character
void Acharacterthatworks::DeleteBobby(Acharacterthatworks* Bobby)
{
	if (GetLocalRole() < ROLE_Authority)
	{
		ServerDeleteBobby(Bobby);
		return;
	}
	BobbyBufferOld->Destroy();
}

//delete character on server, authority
void Acharacterthatworks::ServerDeleteBobby_Implementation(Acharacterthatworks* Bobby)
{
	DeleteBobby(Bobby);
}

//Spawnbobby spectator server implementation, authority
void Acharacterthatworks::ServerSpawnBobbyDefault_Implementation(ASoccerPlayerController* TheNewController)
{
	SpawnBobbyDefault(TheNewController);
}

//spawn spectator bobby
void Acharacterthatworks::SpawnBobbyDefault(ASoccerPlayerController* TheNewController)
{
	if (GetLocalRole() < ROLE_Authority)
	{
		ServerSpawnBobbyDefault(TheNewController);
		SetRole(ROLE_AutonomousProxy);
		return;
	}
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
	TheNewController->ClientSetRotation(FRotator(0.f, 0.f, 0.0f), true);
	Destroy();
}

//spawn bobby red, authority
void Acharacterthatworks::ServerSpawnBobbyRed_Implementation(ASoccerPlayerController* TheNewController)
{
	SpawnBobbyRed(TheNewController);
}

//spawn bobby red
void Acharacterthatworks::SpawnBobbyRed(ASoccerPlayerController* TheNewController)
{
	if (GetLocalRole() < ROLE_Authority)
	{
		ServerSpawnBobbyRed(TheNewController);
		return;
	}
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
	TheNewController->ClientSetRotation(FRotator(0.f, 90.f, 0.0f), true);
	Destroy();
}

//spawn bobby green, authority
void Acharacterthatworks::ServerSpawnBobbyGreen_Implementation(ASoccerPlayerController* TheNewController)
{
	SpawnBobbyGreen(TheNewController);
}

//spawn bobby green
void Acharacterthatworks::SpawnBobbyGreen(ASoccerPlayerController* TheNewController)
{
	if (GetLocalRole() < ROLE_Authority)
	{
		ServerSpawnBobbyGreen(TheNewController);
		return;
	}
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
	TheNewController->ClientSetRotation(FRotator(0.f, -90.f, 0.0f), true);
	Destroy();
}

//set the bobby buffer
void Acharacterthatworks::SetBobbyBuffer(Acharacterthatworks* Bobby)
{
	BobbyBuffer = Bobby;
}

//return the bobby buffer
Acharacterthatworks* Acharacterthatworks::GetBobbyBuffer()
{
	return BobbyBuffer;
}

//clear the bobby buffer
void Acharacterthatworks::ClearBobbyBuffer()
{
	Acharacterthatworks::SetBobbyBuffer(nullptr);
}

//determine which bobby should spawn
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

//power up implementation
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

//power up serverside, actual position
void Acharacterthatworks::ServerUsePowerUp_Implementation()
{
	ClientUsePowerUp();
	LaunchCharacter(FVector(0.f, 0.f, PowerUpStrenght), false, true);
}

//power up clientside, smooth movement
void Acharacterthatworks::ClientUsePowerUp_Implementation()
{
	LaunchCharacter(FVector(0.f, 0.f, PowerUpStrenght), false, true);
}

//player has power up again
void Acharacterthatworks::ClientResetPowerUp_Implementation()
{
	bHasPowerUp = true;
}

//Hit Detection
void Acharacterthatworks::PowerUpCollected()
{
	ClientResetPowerUp();
}

//Delay time for the power up to spawn
void Acharacterthatworks::ServerSpawnPowerUpDelay_Implementation()
{
	ServerSpawnPowerUp(SpawnTransformPowerUp);
}

//spawn power up
void Acharacterthatworks::ServerSpawnPowerUp_Implementation(FTransform Power)
{
	FActorSpawnParameters SpawnParams;
	GetWorld()->SpawnActor<APowerUp>(PowerUpBP, Power, SpawnParams);
}

//return power up status
bool Acharacterthatworks::HasPowerUp()
{
	return bHasPowerUp;
}

//set the player name
void Acharacterthatworks::SetBobbyName(const FText& LeName, ASoccerPlayerController* BobbyController)
{
	if (GetLocalRole() < ROLE_Authority)
	{
		ServerSetBobbyName(LeName, BobbyController);
		return;
	}
	BobbyNameText = LeName;
}

//set the player name serverside
void Acharacterthatworks::ServerSetBobbyName_Implementation(const FText& LeName,ASoccerPlayerController* BobbyController)
{
	BobbyNameText = LeName;
}

//useless
void Acharacterthatworks::MultiSetBobbyName_Implementation(const FText& LeName, ASoccerPlayerController* BobbyController)
{
}


