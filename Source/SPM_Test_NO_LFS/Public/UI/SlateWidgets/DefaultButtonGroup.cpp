#include "DefaultButtonGroup.h"
#include "DefaultButton.h"

void SDefaultButtonGroupWidget::Construct(const FArguments& InArgs)
{
	ButtonGroupData = InArgs._InButtonGroupData;
	TSharedRef<SStackBox> ButtonGroupStackBox = SNew(SStackBox).Orientation(ButtonGroupData.ButtonGroupOrientation);

	ChildSlot
	[
		ButtonGroupStackBox
	];

	SetButtonGroupData(ButtonGroupData);
}

void SDefaultButtonGroupWidget::SetButtonGroupData(const FButtonGroupData& InButtonGroupData)
{
	if (ChildSlot.GetWidget()->GetType() != TEXT("SStackBox"))
	{
		return;
	}
	TSharedRef<SStackBox> StackBox = StaticCastSharedRef<SStackBox>(ChildSlot.GetWidget());
	StackBox->ClearChildren();
	ButtonGroupData = InButtonGroupData;

	EHorizontalAlignment HorizontalAlignment = ButtonGroupData.ButtonGroupAlignmentData.HorizontalAlignment;
	EVerticalAlignment VerticalAlignment = ButtonGroupData.ButtonGroupAlignmentData.VerticalAlignment;

	for (const FButtonData& ButtonData : ButtonGroupData.ButtonDataArray)
	{
		TSharedRef<SDefaultButtonWidget> ButtonWidget = SNew(SDefaultButtonWidget)
			.InButtonData(ButtonData);

		StackBox->AddSlot()
		        .HAlign(HorizontalAlignment)
		        .VAlign(VerticalAlignment)
		        .Padding(ButtonGroupData.SlotPadding).AutoSize()
		[
			ButtonWidget
		];
	}
	StackBox->SetOrientation(ButtonGroupData.ButtonGroupOrientation);
}
