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

	const FMargin ContentPadding = FMargin(425.0f, 225.0f);
	const FMargin ButtonPadding = FMargin(15.f);

	const FText TitleText = LOCTEXT("OptionsTitle", "Options");
	const FText BobbyName = LOCTEXT("BobbyName", "Player");
	const FText SetBobbyName = LOCTEXT("SaveBobbyName", "Save");
	const FText Address = LOCTEXT("Address", "enter an IP address");
	const FText Connect = LOCTEXT("Connect", "Connect");
	const FText ResetPoints = LOCTEXT("ResetPoints(Server only)", "ResetPoints(Server only)");
	const FText Back = LOCTEXT("back", "back");
	const FText FrameLimit = LOCTEXT("Frame Limit", "Frame Limit");


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
			.HintText(Address)
		.Font(ButtonTextStyle)
		.Justification(ETextJustify::Center)
		]
		]
	//Button Save Name
	+ SVerticalBox::Slot()
		.Padding(ButtonPadding)
		[
			SNew(SButton)
			.OnClicked(this, &SOVideoOptionsWidget::OnSetBobbyNameClicked)
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
			.OnClicked(this, &SOVideoOptionsWidget::OnConnectClicked)
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
			
		}
	}
	return FReply::Handled();
}


//save the bobby name
FReply SOVideoOptionsWidget::OnSetBobbyNameClicked() const
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
FReply SOVideoOptionsWidget::OnConnectClicked() const
{
	if (OwningHUD.IsValid())
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

