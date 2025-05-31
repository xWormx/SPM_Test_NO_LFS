#include "StyleSetData.h"

#include "Styling/CoreStyle.h"
#include "Styling/SlateStyleRegistry.h"

#define BOX_BRUSH(RelativePath, ...) FSlateBoxBrush(StyleSet->RootToContentDir(RelativePath, TEXT(".svg")), __VA_ARGS__)
#define IMAGE_BRUSH_SVG(RelativePath, ...) FSlateVectorImageBrush(StyleSet->RootToContentDir(RelativePath, TEXT(".svg")), __VA_ARGS__)
#define TRANSPARENT_ROUNDED_BOX_BRUSH(BorderColor) FSlateRoundedBoxBrush(FLinearColor::Transparent, 4.0f, FLinearColor(BorderColor), 2.0f) //FillColor, Border Radius, BorderColor (param), BorderThickness


TUniquePtr<FSlateStyleSet> FStyleSetData::StyleSet;

void FStyleSetData::Initialize()
{
	if (StyleSet.IsValid())
	{
		return;
	}

	StyleSet = MakeUnique<FSlateStyleSet>(GetStyleSetName());

	// Set paths to content folders to support loading assets that ship with the engine
	StyleSet->SetContentRoot(FPaths::EngineContentDir() / TEXT("Editor/Slate"));
	StyleSet->SetCoreContentRoot(FPaths::EngineContentDir() / TEXT("Slate"));


	InitButtonStyles();
	InitTextBlockStyles();

	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());
}

void FStyleSetData::Shutdown()
{
	if (!StyleSet.IsValid())
	{
		return;
	}
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet.Get());
	StyleSet.Reset();
}

FName FStyleSetData::GetStyleSetName()
{
	static const FName StyleSetName(TEXT("SPMStyleSet"));
	return StyleSetName;
}

void FStyleSetData::InitButtonStyles()
{
	const char* RelativePath = "Common/ButtonHoverHint";
	FMargin Margin = FMargin(4.0f);

	FLinearColor NormalColor = FLinearColor(0.1f, 0.1f, 0.1f, 1.0f);
	FLinearColor HoveredColor = FLinearColor(0.25f, 0.25f, 0.25f, 1.0f);
	FLinearColor PressedColor = FLinearColor(0.5f, 0.5f, 0.5f, 1.0f);
	FLinearColor DisabledColor = FLinearColor(0.4f, 0.4f, 0.4f, 0.5f);

	FSlateRoundedBoxBrush NormalBrush = TRANSPARENT_ROUNDED_BOX_BRUSH(NormalColor);
	FSlateBoxBrush HoveredBrush = BOX_BRUSH(RelativePath, Margin, HoveredColor);
	FSlateBoxBrush PressedBrush = BOX_BRUSH(RelativePath, Margin, PressedColor);
	FSlateRoundedBoxBrush DisabledBrush = TRANSPARENT_ROUNDED_BOX_BRUSH(DisabledColor);

	FButtonStyle MenuButton= FButtonStyle().SetNormal(NormalBrush).SetHovered(HoveredBrush).SetPressed(PressedBrush).SetDisabled(DisabledBrush);
	FName MenuButtonStyleName = StyleNames::MenuButton();

	StyleSet->Set(MenuButtonStyleName, MenuButton);
}

void FStyleSetData::InitTextBlockStyles()
{
	FString FontPath = FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf");
	FLinearColor Foreground = FLinearColor::Yellow;
	FLinearColor ShadowColor = FLinearColor(0.0f, 0.0f, 0.0f, 0.5f);
	FVector2f ShadowOffset(0.0f, 1.0f);

	FName MenuButtonTextStyleName = StyleNames::MenuButtonText();
	StyleSet->Set(MenuButtonTextStyleName,
	              FTextBlockStyle()
	              .SetFont(FSlateFontInfo(FontPath, 32))
	              .SetColorAndOpacity(Foreground)
	              .SetShadowColorAndOpacity(ShadowColor)
	              .SetShadowOffset(ShadowOffset));

	FName MenuHeaderTextStyleName = StyleNames::MenuHeaderText();
	StyleSet->Set(MenuHeaderTextStyleName,
	              FTextBlockStyle()
	              .SetFont(FSlateFontInfo(FontPath, 40))
	              .SetColorAndOpacity(Foreground)
	              .SetShadowColorAndOpacity(ShadowColor)
	              .SetShadowOffset(ShadowOffset));

}

FString FStyleSetData::RootToProjectContentDir(const FString& RelativePath, const TCHAR* Extension)
{
	static const FString ContentDir = FPaths::ProjectConfigDir();
	return (ContentDir / RelativePath) + Extension;
}
