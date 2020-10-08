// Fill out your copyright notice in the Description page of Project Settings.


#include "SoccerGameMode.h"
#include "characterthatworks.h"
#include "SoccerPlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "SoccerGameState.h"


ASoccerGameMode::ASoccerGameMode()
{

	//set default pawn class to our Blueprinted character
	

	PointsToWin = 999;
	PlayerControllerClass = ASoccerPlayerController::StaticClass();
	
	
}




void ASoccerGameMode::OnGreenGoalHit()
{
		ASoccerGameState::PointsRed++;
	
}

void ASoccerGameMode::OnRedGoalHit()
{
	ASoccerGameState::PointsGreen++;
	
	
}

