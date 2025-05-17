// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/SGDeveloperSettings.h"

FName USGDeveloperSettings::GetCategoryName() const
{
	return TEXT("Project");
}
#if WITH_EDITOR
FText USGDeveloperSettings::GetSectionText() const
{
	return NSLOCTEXT("SGDeveloperSettings", "SGSystemSettings", "Custom: SG System Settings");
}

FText USGDeveloperSettings::GetSectionDescription() const
{
	return NSLOCTEXT("SGDeveloperSettings", "SGSystemSettingsDescription", "Everything related to the game's core logic.");
}
#endif