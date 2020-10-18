// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SoccerPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SHITGAEM2_API ASoccerPlayerController : public APlayerController
{
	GENERATED_BODY()

private:
	int Team;

	FText BobbyName;
protected:

	ASoccerPlayerController();
	
	virtual void SetupInputComponent() override;

	void OpenMenu();
	

public:
	void SetTeamGreen();
	void SetTeamRed();
	void SetTeamSpectator();
	void RespawnOnGoal();
	int GetTeam();
	void SetBobbyName(FText& Name);
	FText ReturnBobbyName();



};
