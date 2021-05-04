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



	FReply OnSetBobbyNameClicked() const;
	FReply OnConnectClicked() const;
	FReply OnBackClicked() const;

	virtual FReply OnPreviewKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;


	TSharedPtr<SEditableTextBox> BoxyPtr;
	TSharedPtr<SEditableTextBox> BoxyPtr2;
	TWeakObjectPtr<class AMenuHUD> OwningHUD;

	virtual bool SupportsKeyboardFocus() const override { return true; };

};
