// Fill out your copyright notice in the Description page of Project Settings.


#include "Goalred.h"

// Sets default values
AGoalred::AGoalred()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GoalBounds = CreateDefaultSubobject<UStaticMeshComponent>("GoalBounds");
	SetRootComponent(GoalBounds);
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

