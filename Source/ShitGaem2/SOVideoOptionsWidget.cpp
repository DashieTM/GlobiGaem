// 2020TMCR

#include "SOVideoOptionsWidget.h"
#include "SoccerPlayerController.h"
#include "GameFramework/PlayerController.h"
#include "MenuHUD.h"
#include "characterthatworks.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Engine.h"
#include "Framework/Application/SlateApplication.h"
#include "SoccerGameState.h"
#include "GameFramework/GameUserSettings.h"

#define LOCTEXT_NAMESPACE "OptionsMenu"

void SOVideoOptionsWidget::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;

	OwningHUD = InArgs._OwningHUD;

	const FMargin ContentPadding = FMargin(500.0f, 300.0f);
	const FMargin ButtonPadding = FMargin(10.f);

	const FText TitleText = LOCTEXT("OptionsTitle", "Options");
	const FText VsyncOn = LOCTEXT("Vsync ON", "Vsync ON");
	const FText FullscreenOn = LOCTEXT("Fullscreen", "Fullscreen");
	const FText VsyncOff = LOCTEXT("Vsync OFF", "Vsync OFF");
	const FText FullscreenOff = LOCTEXT("Windowed", "Windowed");
	const FText Address = LOCTEXT("Address", "enter an IP address");
	const FText Connect = LOCTEXT("Connect", "Connect");
	const FText ResetPoints = LOCTEXT("ResetPoints(Server only)", "ResetPoints(Server only)");
	const FText Back = LOCTEXT("back", "back");
	const FText FrameLimit = LOCTEXT("Frame Limit", "Frame Limit");


	FSlateFontInfo ButtonTextStyle = FCoreStyle::Get().GetFontStyle("EmbossedText");
	ButtonTextStyle.Size = 29.f;

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
	//Frame Limiter
	+ SVerticalBox::Slot()
		.Padding(ButtonPadding)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		[
			SNew(SButton)
			.OnClicked(this, &SOVideoOptionsWidget::OnFrameLimitClicked)
		[
			SNew(STextBlock)
			.Font(ButtonTextStyle)
		.Text(FrameLimit)
		.Justification(ETextJustify::Center)
		]
		]

	+ SHorizontalBox::Slot()
		.Padding(ButtonPadding)
		[
			SAssignNew(FrameBox, SEditableTextBox)
			.HintText(FrameLimit)
		.Font(ButtonTextStyle)
		.Justification(ETextJustify::Center)
		]
		]
	//VSync on and off
	+ SVerticalBox::Slot()
		.Padding(ButtonPadding)
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
		[
			SNew(SButton)
			.OnClicked(this, &SOVideoOptionsWidget::OnVSyncClicked)
		[
			SNew(STextBlock)
			.Font(ButtonTextStyle)
		.Text(VsyncOn)
		.Justification(ETextJustify::Center)
		]
		]
			+ SHorizontalBox::Slot()
				[
				SNew(SButton)
				.OnClicked(this, &SOVideoOptionsWidget::OnVSyncOFFClicked)
				[
					SNew(STextBlock)
					.Font(ButtonTextStyle)
				.Text(VsyncOff)
				.Justification(ETextJustify::Center)
				]
		]
		]
	//fullscreen on and off
	+ SVerticalBox::Slot()
		.Padding(ButtonPadding)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		[
			SNew(SButton)
			.OnClicked(this, &SOVideoOptionsWidget::OnFullscreenClicked)
		[
			SNew(STextBlock)
			.Font(ButtonTextStyle)
		.Text(FullscreenOn)
		.Justification(ETextJustify::Center)
		]
		]
	+ SHorizontalBox::Slot()
		[
			SNew(SButton)
			.OnClicked(this, &SOVideoOptionsWidget::OnFullscreenOFFClicked)
		[
			SNew(STextBlock)
			.Font(ButtonTextStyle)
		.Text(FullscreenOff)
		.Justification(ETextJustify::Center)
		]
		]
		]
	//Button Quit
	+ SVerticalBox::Slot()
		.Padding(ButtonPadding)
		[
			SNew(SButton)
			.OnClicked(this, &SOVideoOptionsWidget::OnBackClicked)
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

FReply SOVideoOptionsWidget::OnFrameLimitClicked() const
{
	if (OwningHUD.IsValid())
	{
		if (APlayerController* PC = OwningHUD->PlayerOwner)
		{
			FString Command = FrameBox->GetText().ToString();
			if(Command.IsNumeric())
			{ 
			float limit = FCString::Atof(*Command);
			GEngine->GetGameUserSettings()->SetFrameRateLimit(limit);
			GEngine->GetGameUserSettings()->ApplySettings(true);
			}
		}
	}
	return FReply::Handled();
}


//vsync on
FReply SOVideoOptionsWidget::SOVideoOptionsWidget::OnVSyncClicked() const
{
	if (OwningHUD.IsValid())
	{
		if (APlayerController* PC = OwningHUD->PlayerOwner)
		{
				GEngine->GetGameUserSettings()->SetVSyncEnabled(true);
				GEngine->GetGameUserSettings()->ApplySettings(true);
		}
	}
	return FReply::Handled();
}
//vsync off
FReply SOVideoOptionsWidget::SOVideoOptionsWidget::OnVSyncOFFClicked() const
{
	if (OwningHUD.IsValid())
	{
		if (APlayerController* PC = OwningHUD->PlayerOwner)
		{
			GEngine->GetGameUserSettings()->SetVSyncEnabled(false);
			GEngine->GetGameUserSettings()->ApplySettings(true);
		}
	}
	return FReply::Handled();
}



//fullscreen on
FReply SOVideoOptionsWidget::SOVideoOptionsWidget::OnFullscreenClicked() const
{
	if (OwningHUD.IsValid())
	{
		if (APlayerController* PC = OwningHUD->PlayerOwner)
		{
				GEngine->GetGameUserSettings()->SetFullscreenMode(EWindowMode::Fullscreen);
		}
	}
	GEngine->GetGameUserSettings()->ApplySettings(true);
	return FReply::Handled();
}

//fullscreen off
FReply SOVideoOptionsWidget::SOVideoOptionsWidget::OnFullscreenOFFClicked() const
{
	if (OwningHUD.IsValid())
	{
		if (APlayerController* PC = OwningHUD->PlayerOwner)
		{
				GEngine->GetGameUserSettings()->SetFullscreenMode(EWindowMode::Windowed);
		}
	}
	GEngine->GetGameUserSettings()->ApplySettings(true);
	return FReply::Handled();
}



//return to menu
FReply SOVideoOptionsWidget::OnBackClicked() const
{
	OwningHUD->RemoveVideoOptions();
	return FReply::Handled();
}

//implement the back function with the escape and delete key
FReply SOVideoOptionsWidget::OnPreviewKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == FKey("Escape") || InKeyEvent.GetKey() == FKey("Delete"))
	{
		OwningHUD->RemoveVideoOptions();
	}
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE

