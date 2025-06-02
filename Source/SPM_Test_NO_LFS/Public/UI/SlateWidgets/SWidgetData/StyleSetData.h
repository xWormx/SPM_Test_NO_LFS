#pragma once
#include "CoreMinimal.h"
#include "Styling/SlateStyle.h"

namespace StyleNames
{
	static FName MenuButton()
	{
		static FName Name = FName("MenuButtonStyle");
		return Name;
	}
	static FName MenuPrimaryButton()
	{
		static FName Name = FName("MenuPrimaryButtonStyle");
		return Name;
	}
	static FName MenuButtonText()
	{
		static FName Name = FName("MenuButtonTextStyle");
		return Name;
	}
	static FName MenuPrimaryButtonText()
	{
		static FName Name = FName("MenuPrimaryButtonTextStyle");
		return Name;
	}
	static FName MenuHeaderText()
	{
		static FName Name = FName("MenuHeaderTextStyle");
		return Name;
	}

}

class FStyleSetData final
{
public:
	static void Initialize();
	static void Shutdown();

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
};
