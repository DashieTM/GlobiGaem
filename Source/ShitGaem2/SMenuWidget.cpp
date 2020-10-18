// Fill out your copyright notice in the Description page of Project Settings.


#include "SMenuWidget.h"
#include "SoccerPlayerController.h"
#include "GameFramework/PlayerController.h"
#include "MenuHUD.h"
#include "characterthatworks.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Engine.h"
#include "Framework/Application/SlateApplication.h"

#define LOCTEXT_NAMESPACE "MainMenu"

void SMenuWidget::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;
	

	OwningHUD = InArgs._OwningHUD;

	const FMargin ContentPadding = FMargin(425.0f, 225.0f);
	const FMargin ButtonPadding = FMargin(15.f);

	const FText TitleText = LOCTEXT("GameTitle", "Shit Gaem");
	const FText Level1Text = LOCTEXT("StadiumSmall", "small stadium");
	const FText Level2Text = LOCTEXT("StadiumBig", "big stadium");
	const FText QuitText = LOCTEXT("Quit", "Quit");
	const FText OptionsText = LOCTEXT("Close Menu", "Close Menu");
	const FText MultiplayerText = LOCTEXT("Options", "Options");
	const FText HintTextAddress = LOCTEXT("enter an IP address", "enter an IP address");
	const FText TeamGreen = LOCTEXT("green", "green");
	const FText TeamRed = LOCTEXT("red", "red");
	const FText SpectatorTeam = LOCTEXT("spectator", "spectator");


	FSlateFontInfo ButtonTextStyle = FCoreStyle::Get().GetFontStyle("EmbossedText");
	ButtonTextStyle.Size = 25.f;

	FSlateFontInfo TitleTextStyle = ButtonTextStyle;
	TitleTextStyle.Size = 38.0f;

	ChildSlot
		[
			SNew(SOverlay)
			+SOverlay::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
		
			
			+SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.Padding(ContentPadding)
			[
				SNew(SVerticalBox)

				//Title Text
				+ SVerticalBox::Slot()
				[
					SNew(STextBlock)
					.Font(TitleTextStyle)
					.Text(TitleText)
					.Justification(ETextJustify::Center)
				
				]
				//Button small Stadium
				+SVerticalBox::Slot()
				.Padding(ButtonPadding)
				[
					SNew(SButton)
					.OnClicked(this, &SMenuWidget::OnLevel1Clicked)
					[
						SNew(STextBlock)
						.Font(ButtonTextStyle)
						.Text(Level1Text)
						.Justification(ETextJustify::Center)
					
					]
				]
				//Button big Stadium
				+ SVerticalBox::Slot()
				.Padding(ButtonPadding)
				[
					SNew(SButton)
					.OnClicked(this, &SMenuWidget::OnLevel2Clicked)
					[
						SNew(STextBlock)
						.Font(ButtonTextStyle)
						.Text(Level2Text)
						.Justification(ETextJustify::Center)
					
					]
				]
				//Button Multiplayer
				+ SVerticalBox::Slot()
					.Padding(ButtonPadding)
					[
						SNew(SButton)
						.OnClicked(this, &SMenuWidget::OnMultiplayerClicked)
					[
						SNew(STextBlock)
						.Font(ButtonTextStyle)
					.Text(MultiplayerText)
					.Justification(ETextJustify::Center)

					]
				]
				//Buttons for teams
				+ SVerticalBox::Slot()
				.Padding(ButtonPadding)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					[
						SNew(SButton)
						.OnClicked(this, &SMenuWidget::OnGreenClicked)
						[
							SNew(STextBlock)
							.Font(ButtonTextStyle)
							.Text(TeamGreen)
							.Justification(ETextJustify::Center)
		
						]
					]
					+ SHorizontalBox::Slot()
					[
						SNew(SButton)
						.OnClicked(this, &SMenuWidget::OnRedClicked)
						[
							SNew(STextBlock)
							.Font(ButtonTextStyle)
							.Text(TeamRed)
							.Justification(ETextJustify::Center)

						]
					]
				]
				//Button Spectator
				+ SVerticalBox::Slot()
					.Padding(ButtonPadding)
					[
						SNew(SButton)
						.OnClicked(this, &SMenuWidget::OnSpectatorClicked)
						[
							SNew(STextBlock)
							.Font(ButtonTextStyle)
							.Text(SpectatorTeam)
							.Justification(ETextJustify::Center)

						]
					]
				//Button Quit
				+ SVerticalBox::Slot()
				.Padding(ButtonPadding)
				[
					SNew(SButton)
					.OnClicked(this, &SMenuWidget::OnQuitClicked)
					[
						SNew(STextBlock)
						.Font(ButtonTextStyle)
						.Text(QuitText)
						.Justification(ETextJustify::Center)

					]
				]	
			]
		];
}






FReply SMenuWidget::OnLevel1Clicked() const
{
	if (OwningHUD.IsValid())
	{
		OwningHUD->OpenLevel2();
	}	
		return FReply::Handled();
}

FReply SMenuWidget::OnLevel2Clicked() const
{
	if (OwningHUD.IsValid())
	{
		OwningHUD->OpenLevel1();
	}
		return FReply::Handled();	
}


FReply SMenuWidget::OnMultiplayerClicked() const
{
	OwningHUD->ShowOptions();
	return FReply::Handled();
}

FReply SMenuWidget::OnQuitClicked() const
{
	if (OwningHUD.IsValid())
	{
		if (APlayerController* PC = OwningHUD->PlayerOwner)
		{
			PC->ConsoleCommand("quit");
			
			
		}
	}
	return FReply::Handled();
}

FReply SMenuWidget::OnGreenClicked() const
{
	OwningHUD->SetTeamGreen();
	return FReply::Handled();
}

FReply SMenuWidget::OnRedClicked() const
{
	OwningHUD->SetTeamRed();
	return FReply::Handled();
}

FReply SMenuWidget::OnSpectatorClicked() const
{
	OwningHUD->SetTeamSpectator();
	return FReply::Handled();
}


FReply SMenuWidget::OnPreviewKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == FKey("Escape") || InKeyEvent.GetKey() == FKey("Delete"))
	{
		OwningHUD->RemoveMenu();
		OwningHUD->ShowHUD();
	}
	return FReply::Handled();
}



#undef LOCTEXT_NAMESPACE

