#include "Util/SGUpgradeDeveloperSettings.h"

FName USGUpgradeDeveloperSettings::GetCategoryName() const
{
	return TEXT("Project");
}

#if WITH_EDITOR
FText USGUpgradeDeveloperSettings::GetSectionText() const
{
	return NSLOCTEXT("UpgradeDeveloperSettings", "UpgradeSystemSettings", "Custom: Upgrade System Settings");
}

FText USGUpgradeDeveloperSettings::GetSectionDescription() const
{
	return NSLOCTEXT("UpgradeDeveloperSettings", "UpgradeSystemSettingsDescription", "Everything related to the game's upgrade system.");
}
#endif

