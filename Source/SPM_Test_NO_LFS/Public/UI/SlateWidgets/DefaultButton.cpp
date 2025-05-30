#include "DefaultButton.h"

void SDefaultButtonWidget::Construct(const FArguments& InArgs)
{
	ButtonData = InArgs._InButtonData;

	ChildSlot
	[
		SNew(SButton)
		.OnClicked(ButtonData.OnClicked)
		[
			SNew(STextBlock).Margin(FMargin(ButtonData.DefaultPadding))
			.Text(ButtonData.ButtonText)
			.Justification(ButtonData.ButtonTextJustification)
		]
	];
}

void SDefaultButtonWidget::SetButtonData(const FButtonData& InButtonData)
{
	ButtonData = InButtonData;
	if (ChildSlot.GetWidget()->GetType() == TEXT("SButton"))
	{
		TSharedRef<SButton> Button = StaticCastSharedRef<SButton>(ChildSlot.GetWidget());
		Button->SetOnClicked(ButtonData.OnClicked);

		TSharedRef<STextBlock> TextBlock = StaticCastSharedRef<STextBlock>(Button->GetChildren()->GetChildAt(0));
		TextBlock->SetText(ButtonData.ButtonText);
		TextBlock->SetJustification(ButtonData.ButtonTextJustification);
		TextBlock->SetMargin(FMargin(ButtonData.DefaultPadding));
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
