// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SlateBasics.h"
#include "SlateExtras.h"


class  SOptionsWidget : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SOptionsWidget) {}

	SLATE_ARGUMENT(TWeakObjectPtr<class AMenuHUD>, OwningHUD)

		SLATE_END_ARGS()



		void Construct(const FArguments& InArgs);



	FReply OnSetBobbyNameClicked() const;
	FReply OnConnectClicked() const;
	FReply OnBackClicked() const;
	
	virtual FReply OnPreviewKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;


	TSharedPtr<SEditableTextBox> BoxyPtr;
	TSharedPtr<SEditableTextBox> BoxyPtr2;
	TWeakObjectPtr<class AMenuHUD> OwningHUD;

	virtual bool SupportsKeyboardFocus() const override { return true; };

};
