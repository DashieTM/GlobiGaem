// 2020TMCR

#pragma once

#include "SlateBasics.h"
#include "SlateExtras.h"


class  SOVideoOptionsWidget : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SOVideoOptionsWidget) {}

	SLATE_ARGUMENT(TWeakObjectPtr<class AMenuHUD>, OwningHUD)

		SLATE_END_ARGS()



		void Construct(const FArguments& InArgs);



	FReply OnFrameLimitClicked() const;

	FReply OnVSyncClicked() const;
	FReply OnFullscreenClicked() const;
	FReply OnVSyncOFFClicked() const;
	FReply OnFullscreenOFFClicked() const;
	FReply OnBackClicked() const;

	virtual FReply OnPreviewKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;


	TSharedPtr<SEditableTextBox> BoxyPtr;
	TSharedPtr<SEditableTextBox> BoxyPtr2;
	TSharedPtr<SEditableTextBox> FrameBox;
	TWeakObjectPtr<class AMenuHUD> OwningHUD;

	virtual bool SupportsKeyboardFocus() const override { return true; };

};
