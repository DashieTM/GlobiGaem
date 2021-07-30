// Fill out your copyright notice in the Description page of Project Settings.


#include "SoccerGameState.h"
#include "Net/UnrealNetwork.h"
#include "SoccerPlayerController.h"
#include "GameFramework/PlayerController.h"
#include "characterthatworks.h"
#include "MenuHUD.h"
#include "TimerManager.h"
#include "SoccerGameMode.h"
#include "ball.h"

ASoccerGameState::ASoccerGameState()
{
	PointsRed = 0;
	PointsGreen = 0;
	PlayersGreen = 0;
	PlayersRed = 0;
	RedWinStatus = false;
	GreenWinStatus = false;
	
	bReplicates = true;
}

void ASoccerGameState::BeginPlay()
{
	if(HasAuthority()) Mode = Cast<ASoccerGameMode>(GetWorld()->GetAuthGameMode());
}

void ASoccerGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASoccerGameState, PointsGreen);
	DOREPLIFETIME(ASoccerGameState, PointsRed);
	DOREPLIFETIME(ASoccerGameState, GreenWinStatus);
	DOREPLIFETIME(ASoccerGameState, RedWinStatus);
}

void ASoccerGameState::StartGame()
{
}

void ASoccerGameState::StartGameServer_Implementation()
{
}

void ASoccerGameState::ResetBall()
{
	ResetBallServer();
}

void ASoccerGameState::ResetBallServer_Implementation()
{
	BallInPlay->Destroy();
	GetWorld()->GetTimerManager().SetTimer(MemberTimerHandlestate, this, &ASoccerGameState::StartGame, 3.0f, false);
}

void ASoccerGameState::OnGreenGoalHit()
{
	if(PointsRed <= 1)
	{ 
		PointsRed++;
		return;
	}
	RedWinStatus = true;
	ResetPoints();
}

void ASoccerGameState::OnRedGoalHit()
{
	
	if (PointsGreen <= 1)
	{
		PointsGreen++;
		return;
	}
	GreenWinStatus = true;
	ResetPoints();
}

void ASoccerGameState::PlayersRedIncrease()
{
	
	PlayersRedIncreaseServer();
}

void ASoccerGameState::PlayersRedIncreaseServer_Implementation()
{
	PlayersRed++;
	GetWorld()->GetTimerManager().SetTimer(MemberTimerHandlestate2, this, &ASoccerGameState::SpawnBall, 3.0f, false);
}

void ASoccerGameState::PlayersGreenIncrease()
{
	PlayersGreen++;
	GetWorld()->GetTimerManager().SetTimer(MemberTimerHandlestate3, this, &ASoccerGameState::SpawnBall, 3.0f, false);
}

void ASoccerGameState::SpawnBall()
{
	Mode->SpawnBall();
	GetWorldTimerManager().ClearTimer(MemberTimerHandlestate2);
	GetWorldTimerManager().ClearTimer(MemberTimerHandlestate3);
}

int32 ASoccerGameState::GetPointsRed()
{
	return PointsRed;
}


int32 ASoccerGameState::GetPointsGreen()
{
	return PointsGreen;
}

int32 ASoccerGameState::GetPlayersRed()
{
	return PlayersRed;
}

int32 ASoccerGameState::GetPlayersGreen()
{
	return PlayersGreen;
}

bool ASoccerGameState::RedHasWon()
{
	return RedWinStatus;
}

bool ASoccerGameState::GreenHasWon()
{
	return GreenWinStatus;
}

void ASoccerGameState::GetController()
{
	
	ASoccerPlayerController* Controller = Cast<ASoccerPlayerController>(GetWorld()->GetFirstPlayerController());
	Controller->RespawnOnGoal();
}


void ASoccerGameState::ResetPoints()
{
	PointsGreen = 0;
	PointsRed = 0;
	GetWorld()->GetTimerManager().SetTimer(MemberTimerHandle, this, &ASoccerGameState::ResetVictory, 2.0f, false);
	
}

void ASoccerGameState::ResetVictory()
{

	GreenWinStatus = false;
	RedWinStatus = false;
	GetWorldTimerManager().ClearTimer(MemberTimerHandle);
}