// Fill out your copyright notice in the Description page of Project Settings.


#include "characterthatworks.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Projectile.h"


// Sets default values
Acharacterthatworks::Acharacterthatworks()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

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

	FollowCamera->bUsePawnControlRotation = false;

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

void Acharacterthatworks::ServerShoot_Implementation()
{
	Shoot();
}


void Acharacterthatworks::Shoot()
{
	if(GetLocalRole() < ROLE_Authority)
	{
		ServerShoot();
		return;
	}

	
	FTransform SpawnTransform = FollowCamera->GetComponentTransform();

	SpawnTransform.SetLocation(FollowCamera->GetComponentRotation().Vector() * 400.0f + FollowCamera->GetComponentLocation());
	FActorSpawnParameters SpawnParams;
	
	GetWorld()->SpawnActor<AProjectile>(ProjectileBP, SpawnTransform, SpawnParams);
}

UWorld* Acharacterthatworks::OpenLevel()
{

	return GetWorld();
}