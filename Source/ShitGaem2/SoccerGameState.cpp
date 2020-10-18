// Fill out your copyright notice in the Description page of Project Settings.


#include "SoccerGameState.h"
#include "Net/UnrealNetwork.h"
#include "SoccerPlayerController.h"
#include "GameFramework/PlayerController.h"
#include "characterthatworks.h"
#include "MenuHUD.h"
#include "SoccerGameMode.h"


ASoccerGameState::ASoccerGameState()
{
	PointsRed = 0;
	PointsGreen = 0;

	SetReplicates(true);
	
}

void ASoccerGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASoccerGameState, PointsGreen);
	DOREPLIFETIME(ASoccerGameState, PointsRed);
}

void ASoccerGameState::OnGreenGoalHit()
{
	PointsRed++;
	
}

void ASoccerGameState::OnRedGoalHit()
{
	PointsGreen++;
	
}

int32 ASoccerGameState::GetPointsRed()
{
	return PointsRed;
}


int32 ASoccerGameState::GetPointsGreen()
{
	return PointsGreen;
	
}

void ASoccerGameState::GetController()
{
	
	ASoccerPlayerController* Controller = Cast<ASoccerPlayerController>(GetWorld()->GetFirstPlayerController());
	Controller->RespawnOnGoal();
}


