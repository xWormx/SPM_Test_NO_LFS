#pragma once
#include "CoreMinimal.h"
#include "Styling/SlateStyle.h"

namespace StyleNames
{
	// Text styles
	static FName MenuHeaderText()
	{
		static FName Name = FName("MenuHeaderTextStyle");
		return Name;
	}

	static FName MenuDescriptionText()
	{
		static FName Name = FName("MenuDescriptionTextStyle");
		return Name;
	}

	static FName MenuPrimaryButtonText()
	{
		static FName Name = FName("MenuPrimaryButtonTextStyle");
		return Name;
	}

	static FName MenuSecondaryButtonText()
	{
		static FName Name = FName("MenuSecondaryButtonTextStyle");
		return Name;
	}

	// Button Styles
	static FName MenuPrimaryButton()
	{
		static FName Name = FName("MenuPrimaryButtonStyle");
		return Name;
	}

	static FName MenuSecondaryButton()
	{
		static FName Name = FName("MenuSecondaryButtonStyle");
		return Name;
	}
}

struct FStylingTheme;

class FStyleSetData final
{
public:
	static void Initialize();
	static void Shutdown();
	static void RefreshStyles();

	static ISlateStyle& Get()
	{
		if (!StyleSet.Get())
		{
			Initialize();
		}
		return *StyleSet.Get();
	}

	static FName GetStyleSetName();

private:

	static void InitButtonStyles();
	static void InitTextBlockStyles();

	static FString RootToProjectContentDir(const FString& RelativePath, const TCHAR* Extension);

	static TUniquePtr<FSlateStyleSet> StyleSet;

private:
	static const FStylingTheme* GetCurrentTheme();
};
