#include "DefaultMenu.h"
#include "DefaultButton.h"
#include "SPM_Test_NO_LFS.h"
#include "SWidgetData/StyleSetData.h"

void SDefaultMenu::Construct(const FArguments& InArgs)
{
	TextData = InArgs._InTextData;
	ButtonDataArray = InArgs._InButtonDataArray;

	constexpr float BoxPadding = 10.f;
	EOrientation MenuOrientation = Orient_Vertical;
	EOrientation MenuButtonsOrientation = Orient_Vertical;

	FTextBlockStyle TextStyle = FStyleSetData::Get().GetWidgetStyle<FTextBlockStyle>(TextData.StyleSetName);
	TSharedPtr<STextBlock> MenuTitle = SNew(STextBlock)
		.TextStyle(&TextStyle)
		.Text(TextData.Text)
		.Justification(TextData.TextJustification)
		.Margin(FMargin(TextData.DefaultPadding));

	TSharedPtr<SStackBox> ButtonPanel = SNew(SStackBox).Orientation(MenuButtonsOrientation);
	for (const FButtonData& ButtonData : ButtonDataArray)
	{
		ButtonPanel->AddSlot()
		           .Padding(BoxPadding)
		           .HAlign(HAlign_Fill)
		           .VAlign(VAlign_Center)
		[
			SNew(SDefaultButtonWidget)
			.InButtonData(ButtonData)
		];
	}

	TSharedPtr<SStackBox> MenuPanel = SNew(SStackBox)
		.Orientation(MenuOrientation)
		+ SStackBox::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Bottom)
		.Padding(BoxPadding)
		[
			MenuTitle.ToSharedRef()
		]
		+ SStackBox::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Top)
		.Padding(BoxPadding)
		[
			ButtonPanel.ToSharedRef()
		];

	ChildSlot
	[
		SNew(SOverlay)
		+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SImage)
			.ColorAndOpacity(FLinearColor(0.f, 0.f, 0.f, 1.f))
		]
		+ SOverlay::Slot().Padding(BoxPadding)
		[
			MenuPanel.ToSharedRef()
		]
	];

	ButtonPanel->SetOrientation(MenuButtonsOrientation);
	MenuPanel->SetOrientation(MenuOrientation);
}

void SDefaultMenu::SetTextData(const FTextData& InTextData)
{
	const int32 StackBoxIndex = 1; // The index of the StackBox in the Overlay
	const int32 TextBlockIndex = 0; // The index of the TextBlock in the StackBox

	if (ChildSlot.GetWidget()->GetType() != TEXT("SOverlay"))
	{
		return;
	}

	TSharedRef<SOverlay> Overlay = StaticCastSharedRef<SOverlay>(ChildSlot.GetWidget());
	if (Overlay->GetChildren()->Num() < StackBoxIndex)
	{
		return;
	}

	TSharedRef<SStackBox> StackBox = StaticCastSharedRef<SStackBox>(Overlay->GetChildren()->GetChildAt(StackBoxIndex));
	if (StackBox->GetChildren()->Num() < TextBlockIndex)
	{
		return;
	}

	TSharedRef<STextBlock> TextBlock = StaticCastSharedRef<STextBlock>(
		StackBox->GetChildren()->GetChildAt(TextBlockIndex));
	TextBlock->SetText(InTextData.Text);
	TextBlock->SetJustification(InTextData.TextJustification);
	TextBlock->SetMargin(FMargin(InTextData.DefaultPadding));
}

void SDefaultMenu::SetButtonDataArray(const TArray<FButtonData>& InButtonDataArray)
{
	const int32 StackBoxIndex = 1; // The index of the StackBox in the Overlay
	const int32 ButtonStackBoxIndex = 1; // The index of the TextBlock in the StackBox

	if (ChildSlot.GetWidget()->GetType() != TEXT("SOverlay"))
	{
		return;
	}

	TSharedRef<SOverlay> Overlay = StaticCastSharedRef<SOverlay>(ChildSlot.GetWidget());
	if (Overlay->GetChildren()->Num() < StackBoxIndex)
	{
		return;
	}

	TSharedRef<SStackBox> StackBox = StaticCastSharedRef<SStackBox>(Overlay->GetChildren()->GetChildAt(StackBoxIndex));
	if (StackBox->GetChildren()->Num() < ButtonStackBoxIndex)
	{
		return;
	}

	TSharedRef<SStackBox> BoxPanel = StaticCastSharedRef<SStackBox>(
		StackBox->GetChildren()->GetChildAt(ButtonStackBoxIndex));
	BoxPanel->ClearChildren();
	BoxPanel->SetOrientation(Orient_Vertical);

	for (const FButtonData& ButtonData : InButtonDataArray)
	{
		constexpr float BoxPadding = 10.f;
		BoxPanel->AddSlot().Padding(BoxPadding)
		[
			SNew(SDefaultButtonWidget)
			.InButtonData(ButtonData)
		];
	}
}

/*
void UDefaultMenuWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	CustomWidgetInstance.Reset();
}

void UDefaultMenuWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	if (CustomWidgetInstance)
	{
		CustomWidgetInstance->SetButtonDataArray(ButtonDataArray);
	}
}

TSharedRef<SWidget> UDefaultMenuWidget::RebuildWidget()
{
	CustomWidgetInstance = SNew(SDefaultMenu)
		.InTextData(TextData)
		.InButtonDataArray(ButtonDataArray);

	return CustomWidgetInstance.ToSharedRef();
}

void UDefaultMenuWidget::SetButtonDataArray(const TArray<FButtonData>& InButtonDataArray)
{
	ButtonDataArray = InButtonDataArray;
	SynchronizeProperties();
}
*/
