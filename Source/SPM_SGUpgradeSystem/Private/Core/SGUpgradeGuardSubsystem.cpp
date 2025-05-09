﻿#include "Core/SGUpgradeGuardSubsystem.h"



void USGUpgradeGuardSubsystem::AddToCount(const float Amount)
{
	Count += Amount;
	OnCountAddToCount.Broadcast();
}

void USGUpgradeGuardSubsystem::RemoveFromCount(const float Amount)
{
	Count -= Amount;
	if (Count < 0)
	{
		Count = 0;
	}
}

bool USGUpgradeGuardSubsystem::CanUpgradeBasedOnCount(const float UpgradeCost) const
{
	return Count - UpgradeCost >= 0;
}

float USGUpgradeGuardSubsystem::GetCount() const
{
	return Count;
}
