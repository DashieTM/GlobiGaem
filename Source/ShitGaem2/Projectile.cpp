// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SoccerGameMode.h"
#include "ball.h"
#include "characterthatworks.h"
#include "Misc/App.h"
#include "Goalred.h"
#include "Goalgreen.h"


// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SetReplicateMovement(true);
	bReplicates = true;
	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>("BulletMesh");
	SetRootComponent(BulletMesh);
	BulletMovement = CreateDefaultSubobject<UProjectileMovementComponent>("BulletMovement");
	BulletMovement->InitialSpeed = 8000.0f;
	BulletMovement->MaxSpeed = 8000.0f;

	OnActorHit.AddDynamic(this, &AProjectile::OnProjectileHit);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectile::OnProjectileHit(AActor * SelfActor, AActor * OtherActor, FVector NormalImpulse, const FHitResult & Hit)
{
	if (Aball* Ball = Cast<Aball>(OtherActor))
	{
		return;
	}
	if (Acharacterthatworks* Bobby = Cast<Acharacterthatworks>(OtherActor))
	{
		
		Bobby->LaunchCharacter(FVector(this->GetActorForwardVector().X, this->GetActorForwardVector().Y,this->GetActorForwardVector().Z).GetSafeNormal() * 2000, true, true);
		AProjectile::ProjectileDestroy();
		return;
	}
	AProjectile::ProjectileDestroy();
}

void AProjectile::ProjectileDestroy()
{
	if (GetLocalRole() < ROLE_Authority)
	{
		ServerProjectileDestroy();
		return;
	}
	Destroy();
}

void AProjectile::ServerProjectileDestroy_Implementation()
{
	ProjectileDestroy();
}