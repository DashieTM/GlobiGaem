// Fill out your copyright notice in the Description page of Project Settings.


#include "Goalgreen.h"

// Sets default values
AGoalgreen::AGoalgreen()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	GoalBounds = CreateDefaultSubobject<UStaticMeshComponent>("GoalBounds");
	SetRootComponent(GoalBounds);
}

// Called when the game starts or when spawned
void AGoalgreen::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGoalgreen::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

