#include "DefaultButtonGroup.h"
#include "DefaultButton.h"

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
