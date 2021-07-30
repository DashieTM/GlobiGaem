// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MenuHUD.h"
#include "ball.h"
#include "SoccerGameMode.generated.h"


/**
 * 
 */
UCLASS()
class SHITGAEM2_API ASoccerGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected: 
	
private:


	class Acharacterthatworks* BobbyRed;
	class Acharacterthatworks* BobbyGreen;
	class Acharacterthatworks* BobbyDefault;
	

	FTransform Ball;
	FActorSpawnParameters SpawnParams2;

	class Aball* ShitBall;

	UPROPERTY(EditAnywhere, Category = "Red Bobby")
		TSubclassOf<class Acharacterthatworks> Bobby_Red;

	UPROPERTY(EditAnywhere, Category = "Green Bobby")
		TSubclassOf<class Acharacterthatworks> Bobby_Green;


	UPROPERTY(EditAnywhere, Category = "Default Bobby")
		TSubclassOf<class Acharacterthatworks> Bobby_Default;

	UPROPERTY(EditAnywhere, Category = "Ball")
		TSubclassOf<class Aball> Ball_Default;

	ASoccerGameMode* GM;
	int32 PointsRed;
	int32 PointsGreen;

	

public:
	ASoccerGameMode();
	UPROPERTY(EditAnywhere, Category = "GameSettings")
	int32 PointsToWin;

	void SpawnBall();
	
};
