#include "Core/SGUpgradeGuardGameInstance.h"

void USGUpgradeGuardGameInstance::AddToCount(float Amount)
{
	Count += Amount;
}

void USGUpgradeGuardGameInstance::RemoveFromCount(float Amount)
{
	Count -= Amount;
}

bool USGUpgradeGuardGameInstance::CanUpgradeBasedOnCount(const float UpgradeCost) const
{
	return Count - UpgradeCost >= 0;
}

float USGUpgradeGuardGameInstance::GetCount() const
{
	return Count;
}
