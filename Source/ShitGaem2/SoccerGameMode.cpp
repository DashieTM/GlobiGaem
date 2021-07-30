// Fill out your copyright notice in the Description page of Project Settings.


#include "SoccerGameMode.h"
#include "characterthatworks.h"
#include "SoccerGameState.h"
#include "SoccerPlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "MenuHUD.h"
#include "BobbyPlayerState.h"
#include "ball.h"



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
	ShitBall = nullptr;
}



void ASoccerGameMode::SpawnBall()
{
	if (ShitBall == nullptr)
	{
		Ball.SetScale3D(FVector(1.0f, 1.0f, 1.0f));
		Ball.SetLocation(FVector(0.f, 0.f, 1100.f));
	ShitBall =	GetWorld()->SpawnActor<Aball>(Ball_Default, Ball, SpawnParams2);
	}
	else {
		ShitBall->Destroy();
		Ball.SetScale3D(FVector(1.0f, 1.0f, 1.0f));
		Ball.SetLocation(FVector(0.f, 0.f, 1100.f));
		ShitBall = GetWorld()->SpawnActor<Aball>(Ball_Default, Ball, SpawnParams2);
	}
}