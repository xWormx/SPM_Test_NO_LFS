#include "../../../Public/UI/SlateWidgets/DefaultButtonGroup.h"
#include "../../../Public/UI/SlateWidgets/DefaultButton.h"

void SDefaultButtonGroupWidget::Construct(const FArguments& InArgs)
{
	ButtonGroupData = InArgs._InButtonGroupData;
	ButtonGroupStackBox = SNew(SStackBox).Orientation(ButtonGroupData.Orientation);

	ChildSlot
	[
		ButtonGroupStackBox.ToSharedRef()
	];

	SetButtonGroupData(ButtonGroupData);
}

void SDefaultButtonGroupWidget::SetButtonGroupData(const FButtonGroupData& InButtonGroupData)
{
	ButtonGroupData = InButtonGroupData;

	if (!ButtonGroupStackBox.IsValid())
	{
		ButtonGroupStackBox = SNew(SStackBox).Orientation(ButtonGroupData.Orientation);
		ChildSlot
		[
			ButtonGroupStackBox.ToSharedRef()
		];
	}

	ButtonGroupStackBox->ClearChildren();
	ButtonWidgets.Empty();

	for (const FButtonData& ButtonData : ButtonGroupData.ButtonDataArray)
	{
		TSharedRef<SDefaultButtonWidget> ButtonWidget = SNew(SDefaultButtonWidget)
			.InButtonData(ButtonData);

		ButtonGroupStackBox->AddSlot()
		        .HAlign(ButtonGroupData.Alignment.Horizontal)
		        .VAlign( ButtonGroupData.Alignment.Vertical)
		        .Padding(ButtonGroupData.Margin).AutoSize()
		[
			ButtonWidget
		];
		ButtonWidgets.Add(ButtonWidget);
	}
	ButtonGroupStackBox->SetOrientation(ButtonGroupData.Orientation);
}

TSharedPtr<SDefaultButtonWidget> SDefaultButtonGroupWidget::GetButtonWidget(const int32 Index) const
{
	return (Index >= 0 && Index < ButtonWidgets.Num()) ? ButtonWidgets[Index] : nullptr;
}

void UDefaultButtonGroupWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	CustomWidgetInstance.Reset();
}

void UDefaultButtonGroupWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	if (CustomWidgetInstance)
	{
		CustomWidgetInstance->SetButtonGroupData(ButtonGroupData);
	}
}

TSharedRef<SWidget> UDefaultButtonGroupWidget::RebuildWidget()
{
	CustomWidgetInstance = SNew(SDefaultButtonGroupWidget)
		.InButtonGroupData(ButtonGroupData);
	return CustomWidgetInstance.ToSharedRef();
}

void UDefaultButtonGroupWidget::SetButtonGroupData(const FButtonGroupData& InButtonGroupData)
{
	ButtonGroupData = InButtonGroupData;
	SynchronizeProperties();
}
