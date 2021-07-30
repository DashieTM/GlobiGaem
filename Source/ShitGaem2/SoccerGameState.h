// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ball.h"
#include "SoccerGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "SoccerGameState.generated.h"


/**
 * 
 */
UCLASS()
class SHITGAEM2_API ASoccerGameState : public AGameStateBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(Replicated)
		int32 PlayersGreen;

	UPROPERTY(Replicated)
		int32 PlayersRed;

	UPROPERTY(Replicated)
	int32 PointsGreen;

	UPROPERTY(Replicated)
	int32 PointsRed;

	UPROPERTY(Replicated)
		bool RedWinStatus;

	UPROPERTY(Replicated)
		bool GreenWinStatus;

	int Team;
	class Acharacterthatworks* BobbyRed;
	class Acharacterthatworks* BobbyGreen;
	class Acharacterthatworks* BobbyDefault;
	class Acharacterthatworks* BobbyBuffer;
	class ASoccerGameMode* Mode;

	FTimerHandle MemberTimerHandle;

	FTimerHandle MemberTimerHandlestate;
	FTimerHandle MemberTimerHandlestate2;
	FTimerHandle MemberTimerHandlestate3;

	UPROPERTY(EditAnywhere, Category = "Red Bobby")
		TSubclassOf<class Acharacterthatworks> Bobby_Red;

	UPROPERTY(EditAnywhere, Category = "Green Bobby")
		TSubclassOf<class Acharacterthatworks> Bobby_Green;


	UPROPERTY(EditAnywhere, Category = "Default Bobby")
		TSubclassOf<class Acharacterthatworks> Bobby_Default;
	
	FTransform Ball;
	FActorSpawnParameters SpawnParams2;
	class Aball* BallInPlay;

	UPROPERTY(EditAnywhere, Category = "Default Bobby")
		TSubclassOf<class Aball> Ball_Default;

	
protected:


public:
	ASoccerGameState();	

	void GetController();
	void OnRedGoalHit();
	void OnGreenGoalHit();
	void SetBobbyBuffer(Acharacterthatworks* Bobby, int TeamLocal);
	class Acharacterthatworks* GetBobbyBuffer();
	int GetTeam();
	void ClearBobbyBuffer();
	void ResetPoints();
	void ResetVictory();
	void PlayersGreenIncrease();
	void PlayersRedIncrease();
	void StartGame();
	void ResetBall();
	void SpawnBall();

	UFUNCTION(BlueprintPure, Category = "Players")
		int32 GetPlayersRed();
	UFUNCTION(BlueprintPure, Category = "Players")
		int32 GetPlayersGreen();
	UFUNCTION(BlueprintPure, Category = "Points")
		int32 GetPointsRed();
	UFUNCTION(BlueprintPure, Category = "Points")
		int32 GetPointsGreen();
	UFUNCTION(BlueprintPure, Category = "Points")
		bool RedHasWon();
	UFUNCTION(BlueprintPure, Category = "Points")
		bool GreenHasWon();


	UFUNCTION(Server, Reliable)
		void StartGameServer();
	UFUNCTION(Server, Reliable)
		void ResetBallServer();
	UFUNCTION(Server, Reliable)
		void PlayersRedIncreaseServer();
	
	

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

