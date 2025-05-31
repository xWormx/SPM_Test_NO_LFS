#include "DefaultButton.h"
#include "SWidgetData/StyleSetData.h"

void SDefaultButtonWidget::Construct(const FArguments& InArgs)
{
	ButtonData = InArgs._InButtonData;

	ChildSlot
	[
		ButtonData.CreateButton()
	];
}

void SDefaultButtonWidget::SetButtonData(const FButtonData& InButtonData)
{
	ButtonData = InButtonData;
	if (ChildSlot.GetWidget()->GetType() == TEXT("SButton"))
	{
		TSharedRef<SButton> Button = StaticCastSharedRef<SButton>(ChildSlot.GetWidget());
		InButtonData.ApplyStyle(Button);

		TSharedRef<STextBlock> TextBlock = StaticCastSharedRef<STextBlock>(Button->GetChildren()->GetChildAt(0));
		InButtonData.TextData.ApplyStyle(TextBlock);
	}
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
