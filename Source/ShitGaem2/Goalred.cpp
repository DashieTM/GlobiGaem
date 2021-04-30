// Fill out your copyright notice in the Description page of Project Settings.


#include "Goalred.h"

// Sets default values
AGoalred::AGoalred()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetReplicateMovement(true);
	bReplicates = true;
	GoalBounds = CreateDefaultSubobject<UStaticMeshComponent>("GoalBounds");
	SetRootComponent(GoalBounds);
	Explosion = CreateDefaultSubobject<UParticleSystemComponent>("Explosion");
	Explosion->SetupAttachment(GoalBounds);
	SoundEffect = CreateDefaultSubobject<UAudioComponent>("Sound");
	SoundEffect->SetupAttachment(GoalBounds);

}

// Called when the game starts or when spawned
void AGoalred::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGoalred::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

