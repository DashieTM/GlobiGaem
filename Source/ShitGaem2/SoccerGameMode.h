// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SoccerGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SHITGAEM2_API ASoccerGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASoccerGameMode();

	void OnGreenGoalHit();
	void OnRedGoalHit();

	UPROPERTY(EditAnywhere, Category = "GameSettings")
	int32 PointsToWin;
	
	
};
