// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SoccerGameMode.h"
#include "ball.h"
#include "characterthatworks.h"


// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
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
	
	if (Aball* Target = Cast<Aball>(OtherActor))
	{

	}
	else 
	{
		if (Acharacterthatworks* Target2 = Cast<Acharacterthatworks>(OtherActor))
		{

		}
		else
		{
			Destroy();
		}
	}
	
	
}

