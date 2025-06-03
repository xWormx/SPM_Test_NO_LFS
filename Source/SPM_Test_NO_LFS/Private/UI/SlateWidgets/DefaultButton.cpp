#include "../../../Public/UI/SlateWidgets/DefaultButton.h"

#include "SPM_Test_NO_LFS.h"

void SDefaultButtonWidget::Construct(const FArguments& InArgs)
{
	ButtonData = InArgs._InButtonData;

	ButtonWidget = ButtonData.CreateButton();

	ChildSlot
	[
		ButtonWidget.ToSharedRef()
	];
	ButtonTextWidget = StaticCastSharedRef<STextBlock>(ButtonWidget->GetChildren()->GetChildAt(0));
}

void SDefaultButtonWidget::SetButtonData(const FButtonData& InButtonData)
{
	ButtonData = InButtonData;

	if (ButtonWidget.IsValid())
	{
		TSharedRef<SButton> ButtonWidgetRef = ButtonWidget.ToSharedRef();
		ButtonData.ApplyStyle( ButtonWidgetRef);

		TSharedRef<STextBlock> ButtonTextWidgetRef = ButtonTextWidget.ToSharedRef();
		ButtonData.TextData.ApplyStyle(ButtonTextWidgetRef);
	}
	else
	{
		ButtonWidget = ButtonData.CreateButton();
		ChildSlot
		[
			ButtonWidget.ToSharedRef()
		];
		ButtonTextWidget = StaticCastSharedRef<STextBlock>(ButtonWidget->GetChildren()->GetChildAt(0));
	}
}

TSharedPtr<SButton> SDefaultButtonWidget::GetButtonWidget() const
{
	return ButtonWidget;
}

TSharedPtr<STextBlock> SDefaultButtonWidget::GetButtonTextWidget() const
{
	return ButtonTextWidget;
}

void UDefaultButtonWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	CustomWidgetInstance.Reset();
}

void UDefaultButtonWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (CustomWidgetInstance)
	{
		CustomWidgetInstance->SetButtonData(ButtonData);
	}
}

TSharedRef<SWidget> UDefaultButtonWidget::RebuildWidget()
{
	ButtonData.OnClicked.BindLambda([this]()
	{
		if (OnButtonClicked.IsBound())
		{
			OnButtonClicked.Broadcast();
		}
		return FReply::Handled();
	});

	CustomWidgetInstance = SNew(SDefaultButtonWidget)
		.InButtonData(ButtonData);

	return CustomWidgetInstance.ToSharedRef();
}

void UDefaultButtonWidget::SetButtonData(const FButtonData& InButtonData)
{
	ButtonData = InButtonData;
	SynchronizeProperties();
}
