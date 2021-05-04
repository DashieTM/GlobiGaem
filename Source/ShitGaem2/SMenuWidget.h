// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SlateBasics.h"
#include "SlateExtras.h"


class  SMenuWidget : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SMenuWidget) {}

	SLATE_ARGUMENT(TWeakObjectPtr<class AMenuHUD>, OwningHUD)

		SLATE_END_ARGS()



		void Construct(const FArguments& InArgs);


		
		FReply OnLevel1Clicked() const;
		FReply OnLevel2Clicked() const;
		FReply OnMultiplayerClicked() const;
		FReply OnQuitClicked() const;
		FReply OnRedClicked() const;
		FReply OnGreenClicked() const;
		FReply OnSpectatorClicked() const;
		FReply OnOptionsClicked() const;
		virtual FReply OnPreviewKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;
		
		
	TSharedPtr<SEditableTextBox> BoxyPtr;
	TWeakObjectPtr<class AMenuHUD> OwningHUD;

	virtual bool SupportsKeyboardFocus() const override { return true; };
	
};
