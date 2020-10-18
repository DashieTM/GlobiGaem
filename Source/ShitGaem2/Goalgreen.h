// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Goalgreen.generated.h"


UCLASS()
class SHITGAEM2_API AGoalgreen : public AActor
{
	GENERATED_BODY()
	

public:	
	// Sets default values for this actor's properties
	AGoalgreen();

	UPROPERTY(Editanywhere, Category = "Components")
	UStaticMeshComponent* GoalBounds;

	UPROPERTY(Editanywhere, Category = "Particles")
	UParticleSystemComponent* Explosion;

	UPROPERTY(Editanywhere, Category = "Sound")
		UAudioComponent* SoundEffect;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	

};
