#include "WidgetConstructionData.h"

#include <comdef.h>

void FTextData::ApplyStyle(TSharedRef<STextBlock>& TextBlock) const
{
	TextBlock->SetText(Text);
	TextBlock->SetTextStyle(&TextStyle);
	TextBlock->SetFont(TextStyle.Font);
	TextBlock->SetJustification(TextJustification);
	TextBlock->SetMargin(Margin);
}

TSharedRef<STextBlock> FTextData::CreateTextBlock() const
{
	return SNew(STextBlock)
		.Text(Text)
		.TextStyle(&TextStyle)
		.Font(TextStyle.Font)
		.Justification(TextJustification)
		.Margin(Margin);
}

void FButtonData::ApplyStyle(TSharedRef<SButton>& Button) const
{
	Button->SetButtonStyle(&ButtonStyle);
	Button->SetOnClicked(OnClicked);
	Button->SetPadding(Margin);
}

TSharedRef<SButton> FButtonData::CreateButton() const
{
	return SNew(SButton)
		.ButtonStyle(&ButtonStyle).VAlign( VAlign_Fill).HAlign(HAlign_Fill)
		.OnClicked(OnClicked)
		[
			TextData.CreateTextBlock()
		];
}

void FBackgroundData::ApplyStyle(TSharedRef<SOverlay>& Overlay) const
{
	if (Overlay->GetChildren()->Num() == 0)
	{
		Overlay->AddSlot();
	}
	Overlay->Slot(0)
		.HAlign(BackgroundAlignment.Horizontal)
		.VAlign(BackgroundAlignment.Vertical)
		.Padding(Margin)
		[
			SNew(SImage).ColorAndOpacity(BackgroundColor)
		];
}

TSharedRef<SOverlay> FBackgroundData::CreateBackgroundOverlay() const
{
	return SNew(SOverlay)
		+ SOverlay::Slot()
		.HAlign(BackgroundAlignment.Horizontal)
		.VAlign(BackgroundAlignment.Vertical)
		.Padding(FMargin(Margin))
		[
			SNew(SImage).ColorAndOpacity(BackgroundColor)
		];
}

TSharedRef<SImage> FBackgroundData::CreateBackgroundImage() const
{
	return SNew(SImage).ColorAndOpacity(BackgroundColor);
}
