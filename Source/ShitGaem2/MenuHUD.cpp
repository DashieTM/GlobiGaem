// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuHUD.h"
#include "SMenuWidget.h"
#include "Widgets/SWeakWidget.h"
#include "Engine/Engine.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "SoccerPlayerController.h"
#include "characterthatworks.h"
#include "SoccerGameState.h"
#include "SoccerGameMode.h"
#include "SOptionsWidget.h"
#include "SOVideoOptionsWidget.h"
#include "BobbyPlayerState.h"
#include "Net/UnrealNetwork.h"


void AMenuHUD::BeginPlay()
{
	IsServer = false;
	TheNewController = Cast<ASoccerPlayerController>(PlayerOwner);
	Super::BeginPlay();
	GM = Cast<ASoccerGameState>(GetWorld()->GetGameState());
	BobbyGreenExists = false;
	BobbyRedExists = false;
	BobbyDefaultExists = true;
	BobbyRed = nullptr;
	BobbyGreen = nullptr;
	BobbyBuffer = nullptr;
	BobbyName.FromString("");
	Team = 0;
	SetBobbyBuffer (Cast<Acharacterthatworks>(PlayerOwner->GetCharacter()),0);
	ShowMenu();
	GameState = Cast<ASoccerGameState>(GetWorld()->GetGameState());
}

//show the menu
void AMenuHUD::ShowMenu()
{
	if (CurrentWidget != nullptr)
	{
		CurrentWidget->RemoveFromViewport();
	}
	if (GEngine && GEngine->GameViewport)
	{
		MenuWidget = SNew(SMenuWidget).OwningHUD(this);
		GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(MenuWidgetContainer, SWeakWidget).PossiblyNullContent(MenuWidget.ToSharedRef()));
		FSlateApplication::Get().SetKeyboardFocus(MenuWidget.ToSharedRef());
		if (PlayerOwner)
		{
			PlayerOwner->bShowMouseCursor = true;
			PlayerOwner->SetInputMode(FInputModeUIOnly());
		}
	}
}


//remove the menu
void AMenuHUD::RemoveMenu()
{
	if (GEngine && GEngine->GameViewport && MenuWidgetContainer.IsValid())
	{
		GEngine->GameViewport->RemoveViewportWidgetContent(MenuWidgetContainer.ToSharedRef());
		if (PlayerOwner)
		{
			PlayerOwner->bShowMouseCursor = false;
			PlayerOwner->SetInputMode(FInputModeGameOnly());
			if (PointsHUD != nullptr)
			{
				CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), PointsHUD);
				if (CurrentWidget != nullptr)
				{
					CurrentWidget->AddToViewport();
				}
			}
		}
	}
}

//show options menu
void AMenuHUD::ShowOptions()
{
	if (GEngine && GEngine->GameViewport && MenuWidgetContainer.IsValid())
	{
		GEngine->GameViewport->RemoveViewportWidgetContent(MenuWidgetContainer.ToSharedRef());
		if (PlayerOwner)
		{
			OptionWidget = SNew(SOptionsWidget).OwningHUD(this);
			GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(OptionWidgetContainer, SWeakWidget).PossiblyNullContent(OptionWidget.ToSharedRef()));
			FSlateApplication::Get().SetKeyboardFocus(OptionWidget.ToSharedRef());
		}
	}
}


//remove options and show menu
void AMenuHUD::RemoveOptions()
{
	if (GEngine && GEngine->GameViewport && OptionWidgetContainer.IsValid())
	{
		GEngine->GameViewport->RemoveViewportWidgetContent(OptionWidgetContainer.ToSharedRef());
		if (PlayerOwner)
		{
			MenuWidget = SNew(SMenuWidget).OwningHUD(this);
			GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(MenuWidgetContainer, SWeakWidget).PossiblyNullContent(MenuWidget.ToSharedRef()));
			FSlateApplication::Get().SetKeyboardFocus(MenuWidget.ToSharedRef());
		}
	}
}

//show video options menu
void AMenuHUD::ShowVideoOptions()
{
	if (GEngine && GEngine->GameViewport && MenuWidgetContainer.IsValid())
	{
		GEngine->GameViewport->RemoveViewportWidgetContent(MenuWidgetContainer.ToSharedRef());
		if (PlayerOwner)
		{
			VideoOptionWidget = SNew(SOVideoOptionsWidget).OwningHUD(this);
			GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(VideoOptionWidgetContainer, SWeakWidget).PossiblyNullContent(VideoOptionWidget.ToSharedRef()));
			FSlateApplication::Get().SetKeyboardFocus(VideoOptionWidget.ToSharedRef());
		}
	}
}


//remove video options and show menu
void AMenuHUD::RemoveVideoOptions()
{
	if (GEngine && GEngine->GameViewport && VideoOptionWidgetContainer.IsValid())
	{
		GEngine->GameViewport->RemoveViewportWidgetContent(VideoOptionWidgetContainer.ToSharedRef());
		if (PlayerOwner)
		{
			MenuWidget = SNew(SMenuWidget).OwningHUD(this);
			GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(MenuWidgetContainer, SWeakWidget).PossiblyNullContent(MenuWidget.ToSharedRef()));
			FSlateApplication::Get().SetKeyboardFocus(MenuWidget.ToSharedRef());
		}
	}
}

//open the big stadium
void AMenuHUD::OpenLevel1()
{
	UGameplayStatics::OpenLevel(GetWorld(), "soccer4p");
}

//open the small stadium
void AMenuHUD::OpenLevel2()
{
	UGameplayStatics::OpenLevel(GetWorld(), "soccer_2player");
}

//join the spectator team
void AMenuHUD::SetTeamSpectator()
{
	if (GetWorld()->GetName() == "soccer4p")
	{
		if (Team != 0)
		{
			if (BobbyBuffer)
			{
				Acharacterthatworks* Bobby;
				Acharacterthatworks* BobbyOld;
				BobbyOld = (Cast<Acharacterthatworks>(PlayerOwner->GetCharacter()));
				SetBobbyBuffer(BobbyOld, 0);
				if (!BobbyOld->ReturnisCountdown())
				{
					BobbyOld->Acharacterthatworks::SpawnBobbyDefault(TheNewController);
					Bobby = (Cast<Acharacterthatworks>(PlayerOwner->GetCharacter()));
					TheNewController->SetTeamSpectator();
					GetWorld()->GetTimerManager().SetTimer(MemberTimerHandle, this, &AMenuHUD::ReloadBobbyWSpawn, 0.1f, false);
				}
			}
		}
	}
}

//join the red team
void AMenuHUD::SetTeamRed()
{
	if (GetWorld()->GetName() == "soccer4p")
	{
		if (Team != 2)
		{
			if (BobbyBuffer)
			{
				GameState->PlayersRedIncrease();
				Acharacterthatworks* Bobby;
				Acharacterthatworks* BobbyOld;
				BobbyOld = (Cast<Acharacterthatworks>(PlayerOwner->GetCharacter()));
				SetBobbyBuffer(BobbyOld, 2);
				if (!BobbyOld->ReturnisCountdown())
				{
					BobbyOld->Acharacterthatworks::SpawnBobbyRed(TheNewController, false);
					Bobby = (Cast<Acharacterthatworks>(PlayerOwner->GetCharacter()));
					TheNewController->SetTeamRed();
					GetWorld()->GetTimerManager().SetTimer(MemberTimerHandle, this, &AMenuHUD::ReloadBobbyWSpawn, 0.1f, false);
				}
			}
		}
	}
}

//join the green team
void AMenuHUD::SetTeamGreen()
{
	if (GetWorld()->GetName() == "soccer4p")
	{
		if (Team != 1)
		{
			if (BobbyBuffer)
			{
				GameState->PlayersGreenIncrease();
				Acharacterthatworks* Bobby;
				Acharacterthatworks* BobbyOld;
				BobbyOld = (Cast<Acharacterthatworks>(PlayerOwner->GetCharacter()));
				SetBobbyBuffer(BobbyOld, 1);
				if(!BobbyOld->ReturnisCountdown())
				{ 
				BobbyOld->Acharacterthatworks::SpawnBobbyGreen(TheNewController, false);
				Bobby = (Cast<Acharacterthatworks>(PlayerOwner->GetCharacter()));
				TheNewController->SetTeamGreen();
				GetWorld()->GetTimerManager().SetTimer(MemberTimerHandle, this, &AMenuHUD::ReloadBobbyWSpawn, 0.1f, false);
				}
			}
		}
	}
}

//respawn bobby after goal hit, automatically checks which team the bobby is in
void AMenuHUD::RespawnBobby()
{
	switch(Team)
	{ 
		case 0: 
		{
			break;
		}
		case 1:
		{
			Acharacterthatworks* Bobby;
			Acharacterthatworks* BobbyOld;
			BobbyOld = (Cast<Acharacterthatworks>(PlayerOwner->GetCharacter()));
			SetBobbyBuffer(BobbyOld, 1);
			BobbyOld->Acharacterthatworks::SpawnBobbyGreen(TheNewController, true);
			Bobby = (Cast<Acharacterthatworks>(PlayerOwner->GetCharacter()));
			GetWorld()->GetTimerManager().SetTimer(MemberTimerHandle, this, &AMenuHUD::ReloadBobbyWSpawn, 0.1f, false);
			break;
		}
		case 2:
		{
			Acharacterthatworks* Bobby;
			Acharacterthatworks* BobbyOld;
			BobbyOld = (Cast<Acharacterthatworks>(PlayerOwner->GetCharacter()));
			SetBobbyBuffer(BobbyOld, 2);
			BobbyOld->Acharacterthatworks::SpawnBobbyRed(TheNewController, true);
			Bobby = (Cast<Acharacterthatworks>(PlayerOwner->GetCharacter()));
			GetWorld()->GetTimerManager().SetTimer(MemberTimerHandle, this, &AMenuHUD::ReloadBobbyWSpawn, 0.1f, false);
			break;
		}
	}
}

//set the bobby buffer
void AMenuHUD::SetBobbyBuffer(Acharacterthatworks* Bobby, int TeamLocal)
{
	BobbyBuffer = Bobby;
	Team = TeamLocal;
}

//return the bobby buffer
Acharacterthatworks* AMenuHUD::GetBobbyBuffer()
{
	return BobbyBuffer;
}

//clear the bobby buffer
void AMenuHUD::ClearBobbyBuffer()
{
	AMenuHUD::SetBobbyBuffer(nullptr, 0);
}

//return the bobby team
int AMenuHUD::GetBobbyTeam()
{
	return Team;
}

//reload the bobby after team select, for bobby name
void AMenuHUD::ReloadBobby(FText PlayerName)
{
	Cast<Acharacterthatworks>(TheNewController->GetCharacter())->SetBobbyName(PlayerName, TheNewController);
	SetNameBuffer(PlayerName);
}

//set the bobby name
void AMenuHUD::SetNameBuffer(FText& Name)
{
	TheNewController->SetBobbyName(Name);
}

//get the bobby name
FText AMenuHUD::GetNameBuffer()
{
	return TheNewController->ReturnBobbyName();
}

//reload bobby after goal, because of the name
void AMenuHUD::ReloadBobbyWSpawn()
{
	Cast<Acharacterthatworks>(TheNewController->GetCharacter())->SetBobbyName(GetNameBuffer(), TheNewController);
}