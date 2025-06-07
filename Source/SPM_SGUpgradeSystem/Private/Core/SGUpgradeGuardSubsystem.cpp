#include "Core/SGUpgradeGuardSubsystem.h"


void USGUpgradeGuardSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	ResetCount();
}

void USGUpgradeGuardSubsystem::AddToCount(const float Amount)
{
	Count += Amount;
	OnCountModified.Broadcast();
}

void USGUpgradeGuardSubsystem::RemoveFromCount(const float Amount)
{
	Count -= Amount;
	if (Count < 0)
	{
		Count = 0;
	}
	OnCountModified.Broadcast();
}

bool USGUpgradeGuardSubsystem::CanUpgradeBasedOnCount(const float UpgradeCost) const
{
	return Count - UpgradeCost >= 0;
}

float USGUpgradeGuardSubsystem::GetCount() const
{
	return Count;
}

void USGUpgradeGuardSubsystem::SetCount(float NewCount)
{
	Count = NewCount;
	OnCountModified.Broadcast();
}

void USGUpgradeGuardSubsystem::ResetCount()
{
	Count = DefaultValue;
	OnCountModified.Broadcast();
}
