#include "DefaultMenu.h"

#include "DefaultButton.h"

void SDefaultMenu::Construct(const FArguments& InArgs)
{
	TextData = InArgs._InTextData;
	ButtonDataArray = InArgs._InButtonDataArray;
	constexpr float BoxPadding = 10.f;

	TSharedPtr<SStackBox> BoxPanel = SNew(SStackBox).Orientation(Orient_Vertical);
	for (const FButtonData ButtonData : ButtonDataArray)
	{
		BoxPanel->AddSlot()		        
		        .Padding(BoxPadding)
		[
			SNew(SDefaultButtonWidget)
			.InButtonData(ButtonData)
		];
	}
	ChildSlot
	[
		SNew(SStackBox).Orientation(Orient_Vertical)
		+ SStackBox::Slot()
		.AutoSize()
		.Padding(BoxPadding)
		[
			SNew(STextBlock)
			.Text(TextData.Text)
			.Justification(TextData.ButtonTextJustification)
			.Margin(FMargin(TextData.DefaultPadding))
		]
		+ SStackBox::Slot()
		.AutoSize()
		.Padding(BoxPadding)
		[
			BoxPanel.ToSharedRef()
		]
	];
}

void SDefaultMenu::SetTextData(const FTextData& InTextData)
{
	if (ChildSlot.GetWidget()->GetType() == TEXT("SStackBox"))
	{
		TSharedRef<SStackBox> StackBox = StaticCastSharedRef<SStackBox>(ChildSlot.GetWidget());
		if (StackBox->GetChildren()->Num() > 1)
		{
			TSharedRef<STextBlock> TextBlock = StaticCastSharedRef<STextBlock>(StackBox->GetChildren()->GetChildAt(0));
			TextBlock->SetText(InTextData.Text);
			TextBlock->SetJustification(InTextData.ButtonTextJustification);
			TextBlock->SetMargin(FMargin(InTextData.DefaultPadding));
		}
	}
}

void SDefaultMenu::SetButtonDataArray(const TArray<FButtonData>& InButtonDataArray)
{
	if (ChildSlot.GetWidget()->GetType() != TEXT("SStackBox"))
	{
		return;
	}
	TSharedRef<SStackBox> StackBox = StaticCastSharedRef<SStackBox>(ChildSlot.GetWidget());
	if (StackBox->GetChildren()->Num() > 1)
	{
		TSharedRef<SStackBox> BoxPanel = StaticCastSharedRef<SStackBox>(StackBox->GetChildren()->GetChildAt(1));
		BoxPanel->ClearChildren();
		for (const FButtonData& ButtonData : InButtonDataArray)
		{
			constexpr float BoxPadding = 10.f;
			BoxPanel->AddSlot()			        
			        .Padding(BoxPadding)
			[
				SNew(SDefaultButtonWidget)
				.InButtonData(ButtonData)
			];
		}
	}
}
