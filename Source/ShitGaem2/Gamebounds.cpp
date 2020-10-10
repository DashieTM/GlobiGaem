// Fill out your copyright notice in the Description page of Project Settings.


#include "Gamebounds.h"
#include "Components/BoxComponent.h"

// Sets default values
AGamebounds::AGamebounds()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GameBounds = CreateDefaultSubobject<UBoxComponent>("GameBounds");

}

