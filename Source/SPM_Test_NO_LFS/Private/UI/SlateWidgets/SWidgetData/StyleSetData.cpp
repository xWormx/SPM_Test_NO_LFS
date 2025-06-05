#include "../../../../Public/UI/SlateWidgets/SWidgetData/StyleSetData.h"

#include "SPM_Test_NO_LFS.h"
#include "../../../../Public/UI/SlateWidgets/SWidgetData/StylingTheme.h"
#include "Styling/CoreStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Utils/SGDeveloperSettings.h"

#define BOX_BRUSH(RelativePath, ...) FSlateBoxBrush(StyleSet->RootToContentDir(RelativePath, TEXT(".svg")), __VA_ARGS__)
#define IMAGE_BRUSH_SVG(RelativePath, ...) FSlateVectorImageBrush(StyleSet->RootToContentDir(RelativePath, TEXT(".svg")), __VA_ARGS__)
#define TRANSPARENT_ROUNDED_BOX_BRUSH(BorderColor) FSlateRoundedBoxBrush(FLinearColor::Transparent, 4.0f, FLinearColor(BorderColor), 2.0f) //FillColor, Border Radius, BorderColor (param), BorderThickness
#define ROUNDED_BOX_BRUSH(FillColor, BorderColor) FSlateRoundedBoxBrush(FillColor, 4.0f, FLinearColor(BorderColor), 2.0f) //FillColor, Border Radius, BorderColor (param), BorderThickness
#define CONTENT_FONT_PATH(FileName, Extension) FPaths::ProjectContentDir()/TEXT("CrazyJack/UI/Fonts/TTF/") / FileName + Extension

TUniquePtr<FSlateStyleSet> FStyleSetData::StyleSet;

void FStyleSetData::Initialize()
{
	if (StyleSet.IsValid())
	{
		EMMA_LOG(Warning, TEXT("StyleSet is already initialized."));
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

void FStyleSetData::RefreshStyles()
{
	if (StyleSet.IsValid())
	{
		Shutdown();
	}
	Initialize();
}

FName FStyleSetData::GetStyleSetName()
{
	static const FName StyleSetName(TEXT("SPMStyleSet"));
	return StyleSetName;
}

void FStyleSetData::InitButtonStyles()
{
	const FStylingTheme* Theme = GetCurrentTheme();
	if (!Theme)
	{
		// Fallback to original implementation
		EMMA_LOG(Warning, TEXT("FStyleSetData::InitButtonStyles: No theme found, using default styles."));
		const char* RelativePath = "Common/ButtonHoverHint";
		FMargin Margin = FMargin(4.0f);

		FLinearColor NormalColor = FLinearColor(0.1f, 0.1f, 0.1f, 1.0f);
		FLinearColor HoveredColor = FLinearColor(0.2f, 0.2f, 0.2f, 0.8f);
		FLinearColor PressedColor = FLinearColor(0.15f, 0.15f, 0.15f, 1.0f);
		FLinearColor DisabledColor = FLinearColor(0.4f, 0.4f, 0.4f, 0.5f);

		FSlateRoundedBoxBrush NormalBrush = TRANSPARENT_ROUNDED_BOX_BRUSH(NormalColor);
		FSlateBoxBrush HoveredBrush = BOX_BRUSH(RelativePath, Margin, HoveredColor);
		FSlateBoxBrush PressedBrush = BOX_BRUSH(RelativePath, Margin, PressedColor);
		FSlateRoundedBoxBrush DisabledBrush = TRANSPARENT_ROUNDED_BOX_BRUSH(DisabledColor);

		FName MenuButtonStyleName = StyleNames::MenuSecondaryButton();
		StyleSet->Set(MenuButtonStyleName,
		              FButtonStyle().SetNormal(NormalBrush).SetHovered(HoveredBrush).SetPressed(PressedBrush).
		                             SetDisabled(DisabledBrush));

		FName MenuPrimaryButtonStyleName = StyleNames::MenuPrimaryButton();
		FLinearColor YellowColor = FLinearColor::Yellow;
		StyleSet->Set(MenuPrimaryButtonStyleName, FButtonStyle()
		                                          .SetNormal(TRANSPARENT_ROUNDED_BOX_BRUSH(YellowColor))
		                                          .SetHovered(ROUNDED_BOX_BRUSH(NormalColor, YellowColor)).SetPressed(
			                                          PressedBrush).SetDisabled(DisabledBrush));

		return;
	}

	// Use theme data instead of hardcoding styles
	StyleSet->Set(StyleNames::MenuSecondaryButton(), Theme->SecondaryButton);
	StyleSet->Set(StyleNames::MenuPrimaryButton(), Theme->PrimaryButton);
}

void FStyleSetData::InitTextBlockStyles()
{
	const FStylingTheme* Theme = GetCurrentTheme();
	if (!Theme)
	{
		EMMA_LOG(Warning, TEXT("FStyleSetData::InitTextBlockStyles: No theme found, using default styles."));
		// Fallback to original implementation
		static const TCHAR* TTF = TEXT(".ttf");

		FLinearColor Foreground = FLinearColor::Yellow;
		FLinearColor ShadowColor = FLinearColor(0.0f, 0.0f, 0.0f, 0.5f);
		FVector2f ShadowOffset(0.0f, 1.0f);

		FName MenuButtonTextStyleName = StyleNames::MenuSecondaryButtonText();
		StyleSet->Set(MenuButtonTextStyleName,
		              FTextBlockStyle()
		              .SetFont(FSlateFontInfo(CONTENT_FONT_PATH("Roboto-Thin", TTF), 32))
		              .SetColorAndOpacity(Foreground)
		              .SetShadowColorAndOpacity(ShadowColor)
		              .SetShadowOffset(ShadowOffset));

		FName MenuPrimaryButtonTextStyleName = StyleNames::MenuPrimaryButtonText();
		StyleSet->Set(MenuPrimaryButtonTextStyleName,
		              FTextBlockStyle()
		              .SetFont(FSlateFontInfo(CONTENT_FONT_PATH("Roboto-Condensed", TTF), 32))
		              .SetColorAndOpacity(Foreground)
		              .SetShadowColorAndOpacity(ShadowColor)
		              .SetShadowOffset(ShadowOffset));

		FName MenuHeaderTextStyleName = StyleNames::MenuHeaderText();
		StyleSet->Set(MenuHeaderTextStyleName,
		              FTextBlockStyle()
		              .SetFont(FSlateFontInfo(CONTENT_FONT_PATH("Roboto-Thin", TTF), 40))
		              .SetColorAndOpacity(Foreground)
		              .SetShadowColorAndOpacity(ShadowColor)
		              .SetShadowOffset(ShadowOffset));
		return;
	}

	// Use theme data
	StyleSet->Set(StyleNames::MenuHeaderText(), Theme->Header);
	StyleSet->Set(StyleNames::MenuDescriptionText(), Theme->Description);
	StyleSet->Set(StyleNames::MenuPrimaryButtonText(), Theme->PrimaryButtonText);
	StyleSet->Set(StyleNames::MenuSecondaryButtonText(), Theme->SecondaryButtonText);
}

FString FStyleSetData::RootToProjectContentDir(const FString& RelativePath, const TCHAR* Extension)
{
	static const FString ContentDir = FPaths::ProjectContentDir();
	return (ContentDir / RelativePath) + Extension;
}

const FStylingTheme* FStyleSetData::GetCurrentTheme()
{
	const USGDeveloperSettings* Settings = GetDefault<USGDeveloperSettings>();
	if (!Settings || !Cast<UDataTable>(Settings->StylingThemeDataTable.TryLoad()))
	{
		EMMA_LOG(Error, TEXT("Missing or null StylingThemeDataTable in SGDeveloperSettings"));
		return nullptr;
	}

	UDataTable* DataTable = Cast<UDataTable>(Settings->StylingThemeDataTable.TryLoad());
	FDataTableRowHandle ThemeRowHandle = Settings->StylingThemeRow;
	FString ThemeRowName = ThemeRowHandle.RowName.ToString(); //TODO: Make this configurable or pass as a parameter
	return DataTable->FindRow<FStylingTheme>(FName(*ThemeRowName), TEXT("Looking for theme"));
}

#undef IMAGE_BRUSH_SVG
#undef BOX_BRUSH
#undef TRANSPARENT_ROUNDED_BOX_BRUSH
#undef ROUNDED_BOX_BRUSH
#undef CONTENT_FONT_PATH
