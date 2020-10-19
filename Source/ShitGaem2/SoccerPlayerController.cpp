
#include "SoccerPlayerController.h"
#include "MenuHUD.h"


ASoccerPlayerController::ASoccerPlayerController()
{
	//Team Spectator
	Team = 0;
	BobbyName.FromString("");

}
 

void ASoccerPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (InputComponent)
	{
		InputComponent->BindAction("OpenMenu", IE_Pressed, this, &ASoccerPlayerController::OpenMenu);
	}

}


void ASoccerPlayerController::OpenMenu()
{
	if (AMenuHUD* MenuHUD = Cast<AMenuHUD>(GetHUD()))
	{
		MenuHUD->ShowMenu();
	}
}

void ASoccerPlayerController::SetTeamGreen()
{
	//Team Green
	Team = 1;
}

void ASoccerPlayerController::SetTeamRed()
{
	//Team Red
	Team = 2;
}

void ASoccerPlayerController::SetTeamSpectator()
{
	//Team Spectator
	Team = 0;
}

void ASoccerPlayerController::RespawnOnGoal()
{
	if (AMenuHUD* MenuHUD = Cast<AMenuHUD>(GetHUD()))
	{
		MenuHUD->RespawnBobby();
	}
}

int ASoccerPlayerController::GetTeam()
{
	return Team;
}

void ASoccerPlayerController::SetBobbyName(FText& Name)
{
	BobbyName = Name;
}

FText ASoccerPlayerController::ReturnBobbyName()
{
	return BobbyName;
}