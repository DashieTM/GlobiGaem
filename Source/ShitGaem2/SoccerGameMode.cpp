// Fill out your copyright notice in the Description page of Project Settings.


#include "SoccerGameMode.h"
#include "characterthatworks.h"
#include "SoccerGameState.h"
#include "SoccerPlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "MenuHUD.h"
#include "BobbyPlayerState.h"



ASoccerGameMode::ASoccerGameMode()
{

	//set default pawn class to our Blueprinted character
	PointsRed = 0;
	PointsGreen = 0;
	GM = nullptr;
	PointsToWin = 999;
	PlayerControllerClass = ASoccerPlayerController::StaticClass();
	PlayerStateClass = ABobbyPlayerState::StaticClass();
	GameStateClass = ASoccerGameState::StaticClass();
	HUDClass = AMenuHUD::StaticClass();
	
}

