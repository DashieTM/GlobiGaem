// Fill out your copyright notice in the Description page of Project Settings.


#include "SOptionsWidget.h"
#include "SoccerPlayerController.h"
#include "GameFramework/PlayerController.h"
#include "MenuHUD.h"
#include "characterthatworks.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Engine.h"
#include "Framework/Application/SlateApplication.h"
#include "SoccerGameState.h"

#define LOCTEXT_NAMESPACE "OptionsMenu"

void SOptionsWidget::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;

	OwningHUD = InArgs._OwningHUD;

	const FMargin ContentPadding = FMargin(425.0f, 225.0f);
	const FMargin ButtonPadding = FMargin(15.f);

	const FText TitleText = LOCTEXT("OptionsTitle", "Options");
	const FText BobbyName = LOCTEXT("BobbyName", "Player");
	const FText SetBobbyName = LOCTEXT("SaveBobbyName", "Save");
	const FText Address = LOCTEXT("Address", "enter an IP address");
	const FText Connect = LOCTEXT("Connect", "Connect");
	const FText ResetPoints = LOCTEXT("ResetPoints(Server only)", "ResetPoints(Server only)");
	const FText Back = LOCTEXT("back", "back");


	FSlateFontInfo ButtonTextStyle = FCoreStyle::Get().GetFontStyle("EmbossedText");
	ButtonTextStyle.Size = 25.f;

	FSlateFontInfo TitleTextStyle = ButtonTextStyle;
	TitleTextStyle.Size = 38.0f;

	ChildSlot
	[
		SNew(SOverlay)
		+ SOverlay::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		+ SOverlay::Slot()
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
			//Player Name Text Box
			+ SVerticalBox::Slot()
			.Padding(ButtonPadding)
			[
				SAssignNew(BoxyPtr2, SEditableTextBox)
				.HintText(BobbyName)
				.Font(ButtonTextStyle)
				.Justification(ETextJustify::Center)
			]
			//Button Save Name
			+ SVerticalBox::Slot()
			.Padding(ButtonPadding)
			[
					SNew(SButton)
					.OnClicked(this, &SOptionsWidget::OnSetBobbyNameClicked)
				[
					SNew(STextBlock)
					.Font(ButtonTextStyle)
					.Text(SetBobbyName)
					.Justification(ETextJustify::Center)
				]
			]
			//IP address Box
			+ SVerticalBox::Slot()
			.Padding(ButtonPadding)
			[
				SAssignNew(BoxyPtr, SEditableTextBox)
				.HintText(Address)
				.Font(ButtonTextStyle)
				.Justification(ETextJustify::Center)
			]
			//Connect to IP
			+ SVerticalBox::Slot()
			.Padding(ButtonPadding)
			[
				SNew(SButton)
				.OnClicked(this, &SOptionsWidget::OnConnectClicked)
				[
					SNew(STextBlock)
					.Font(ButtonTextStyle)
					.Text(Connect)
					.Justification(ETextJustify::Center)
				]
			]
			//Button Quit
			+ SVerticalBox::Slot()
			.Padding(ButtonPadding)
			[
				SNew(SButton)
				.OnClicked(this, &SOptionsWidget::OnBackClicked)
				[
					SNew(STextBlock)
					.Font(ButtonTextStyle)
					.Text(Back)
					.Justification(ETextJustify::Center)
				]
			]
		]
	];
}

//save the bobby name
FReply SOptionsWidget::OnSetBobbyNameClicked() const
{
	if (OwningHUD.IsValid())
	{
		if (APlayerController* PC = OwningHUD->PlayerOwner)
		{
			FText PlayerName = BoxyPtr2->GetText();
			OwningHUD->ReloadBobby(PlayerName);
		}
	}
	return FReply::Handled();
}

//connect to entered IP
FReply SOptionsWidget::OnConnectClicked() const
{
	if(OwningHUD.IsValid())
	{
		if (APlayerController* PC = OwningHUD->PlayerOwner)
		{
			FString Command = BoxyPtr->GetText().ToString();
			Command = "open " + Command;
			PC->ConsoleCommand(Command);
		}
	}
	return FReply::Handled();
}


//return to menu
FReply SOptionsWidget::OnBackClicked() const
{
	OwningHUD->RemoveOptions();
	return FReply::Handled();
}

//implement the back function with the escape and delete key
FReply SOptionsWidget::OnPreviewKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == FKey("Escape") || InKeyEvent.GetKey() == FKey("Delete"))
	{
		OwningHUD->RemoveOptions();
	}
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE

