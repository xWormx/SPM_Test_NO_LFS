#include "Core/SGUpgradeGuardSubsystem.h"



void USGUpgradeGuardSubsystem::AddToCount(const float Amount)
{
	Count += Amount;
}

void USGUpgradeGuardSubsystem::RemoveFromCount(const float Amount)
{
	Count -= Amount;
}

bool USGUpgradeGuardSubsystem::CanUpgradeBasedOnCount(const float UpgradeCost) const
{
	return Count - UpgradeCost >= 0;
}

float USGUpgradeGuardSubsystem::GetCount() const
{
	return Count;
}
