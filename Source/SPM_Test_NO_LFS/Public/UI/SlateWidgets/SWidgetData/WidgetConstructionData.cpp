#include "WidgetConstructionData.h"

#include <comdef.h>

void FTextData::ApplyStyle(TSharedRef<STextBlock>& TextBlock) const
{
	TextBlock->SetText(Text);
	TextBlock->SetTextStyle(&TextStyle);
	TextBlock->SetFont(TextStyle.Font);
	TextBlock->SetJustification(TextJustification);
	TextBlock->SetMargin(FMargin(DefaultPadding));
}

TSharedRef<STextBlock> FTextData::CreateTextBlock() const
{
	return SNew(STextBlock)
		.Text(Text)
		.TextStyle(&TextStyle)
		.Font(TextStyle.Font)
		.Justification(TextJustification)
		.Margin(FMargin(DefaultPadding));
}

void FButtonData::ApplyStyle(TSharedRef<SButton>& Button) const
{
	Button->SetButtonStyle(&ButtonStyle);
	Button->SetOnClicked(OnClicked);
	Button->SetPadding(FMargin(DefaultPadding));
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
