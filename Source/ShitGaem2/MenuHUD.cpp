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
}

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


void AMenuHUD::OpenLevel1()
{
	UGameplayStatics::OpenLevel(GetWorld(), "soccer4p");
}

void AMenuHUD::OpenLevel2()
{
	UGameplayStatics::OpenLevel(GetWorld(), "soccer_2player");
}



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
				BobbyOld->Acharacterthatworks::SpawnBobbyDefault(TheNewController);
				Bobby = (Cast<Acharacterthatworks>(PlayerOwner->GetCharacter()));
				TheNewController->SetTeamSpectator();
				TheNewController->ClientSetRotation(FRotator(0.f, 0.f, 0.0f));
				GetWorld()->GetTimerManager().SetTimer(MemberTimerHandle, this, &AMenuHUD::ReloadBobbyWSpawn, 0.1f, false);
			}
		}
	}
}

void AMenuHUD::SetTeamRed()
{
	if (GetWorld()->GetName() == "soccer4p")
	{
		if (Team != 2)
		{
			if (BobbyBuffer)
			{
				Acharacterthatworks* Bobby;
				Acharacterthatworks* BobbyOld;
				BobbyOld = (Cast<Acharacterthatworks>(PlayerOwner->GetCharacter()));
				SetBobbyBuffer(BobbyOld, 2);
				BobbyOld->Acharacterthatworks::SpawnBobbyRed(TheNewController);
				Bobby = (Cast<Acharacterthatworks>(PlayerOwner->GetCharacter()));
				TheNewController->SetTeamRed();
				TheNewController->ClientSetRotation(FRotator(0.f, 90.f, 0.0f));
				GetWorld()->GetTimerManager().SetTimer(MemberTimerHandle, this, &AMenuHUD::ReloadBobbyWSpawn, 0.1f, false);
			}
		}
	}
}

void AMenuHUD::SetTeamGreen()
{
	if (GetWorld()->GetName() == "soccer4p")
	{
		if (Team != 1)
		{
			if (BobbyBuffer)
			{
				
				Acharacterthatworks* Bobby;
				Acharacterthatworks* BobbyOld;
				BobbyOld = (Cast<Acharacterthatworks>(PlayerOwner->GetCharacter()));
				SetBobbyBuffer(BobbyOld, 1);
				BobbyOld->Acharacterthatworks::SpawnBobbyGreen(TheNewController);
				Bobby = (Cast<Acharacterthatworks>(PlayerOwner->GetCharacter()));
				TheNewController->SetTeamGreen();
				
				TheNewController->ClientSetRotation(FRotator(0.f, -90.f, 0.0f));
				GetWorld()->GetTimerManager().SetTimer(MemberTimerHandle, this, &AMenuHUD::ReloadBobbyWSpawn, 0.1f, false);
			}
		}
	}
}


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
			BobbyOld->Acharacterthatworks::SpawnBobbyGreen(TheNewController);
			Bobby = (Cast<Acharacterthatworks>(PlayerOwner->GetCharacter()));
			TheNewController->ClientSetRotation(FRotator(0.f, -90.f, 0.0f));
			GetWorld()->GetTimerManager().SetTimer(MemberTimerHandle, this, &AMenuHUD::ReloadBobbyWSpawn, 0.1f, false);
			break;
		}
		case 2:
		{
			Acharacterthatworks* Bobby;
			Acharacterthatworks* BobbyOld;
			BobbyOld = (Cast<Acharacterthatworks>(PlayerOwner->GetCharacter()));
			SetBobbyBuffer(BobbyOld, 2);
			BobbyOld->Acharacterthatworks::SpawnBobbyRed(TheNewController);
			Bobby = (Cast<Acharacterthatworks>(PlayerOwner->GetCharacter()));
			TheNewController->ClientSetRotation(FRotator(0.f, 90.f, 0.0f));
			GetWorld()->GetTimerManager().SetTimer(MemberTimerHandle, this, &AMenuHUD::ReloadBobbyWSpawn, 0.1f, false);
			break;
		}
	}
}

void AMenuHUD::SetBobbyBuffer(Acharacterthatworks* Bobby, int TeamLocal)
{
	BobbyBuffer = Bobby;
	Team = TeamLocal;
}

Acharacterthatworks* AMenuHUD::GetBobbyBuffer()
{
	return BobbyBuffer;
}

void AMenuHUD::ClearBobbyBuffer()
{
	AMenuHUD::SetBobbyBuffer(nullptr, 0);
}

int AMenuHUD::GetBobbyTeam()
{
	return Team;
}

void AMenuHUD::ReloadBobby(FText PlayerName)
{
	Cast<Acharacterthatworks>(TheNewController->GetCharacter())->SetBobbyName(PlayerName, TheNewController);
	SetNameBuffer(PlayerName);
}

void AMenuHUD::SetNameBuffer(FText& Name)
{
	TheNewController->SetBobbyName(Name);
}

FText AMenuHUD::GetNameBuffer()
{
	return TheNewController->ReturnBobbyName();
}

void AMenuHUD::ReloadBobbyWSpawn()
{
	Cast<Acharacterthatworks>(TheNewController->GetCharacter())->SetBobbyName(GetNameBuffer(), TheNewController);
}