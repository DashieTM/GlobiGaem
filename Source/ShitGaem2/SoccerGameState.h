// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SoccerGameState.generated.h"

/**
 * 
 */
UCLASS()
class SHITGAEM2_API ASoccerGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	ASoccerGameState();	

	static int PointsGreen;
	static int PointsRed;
};

