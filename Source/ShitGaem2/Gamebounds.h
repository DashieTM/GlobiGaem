// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gamebounds.generated.h"


UCLASS()
class SHITGAEM2_API AGamebounds : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGamebounds();

	UPROPERTY(EditAnywhere)
	class UBoxComponent* GameBounds;

};
