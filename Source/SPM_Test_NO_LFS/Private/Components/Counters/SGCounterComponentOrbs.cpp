#include "Components/Counters/SGCounterComponentOrbs.h"
#include "Core/SGUpgradeSubsystem.h"
#include "Pickups/SGPickUpOrbs.h"

USGCounterComponentOrbs::USGCounterComponentOrbs()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USGCounterComponentOrbs::BeginPlay()
{
	Super::BeginPlay();
	
	if (USGUpgradeSubsystem* UpgradeSubsystem = GetOwner()->GetGameInstance()->GetSubsystem<USGUpgradeSubsystem>())
	{
		UpgradeSubsystem->OnUpgradeCost.AddDynamic(this, &USGCounterComponentOrbs::RemoveOrbs);
	}
}

float USGCounterComponentOrbs::GetOrbCount() const
{
	return OrbCount;
}

void USGCounterComponentOrbs::AddOrbs(const float Amount)
{
	OrbCount += Amount;
}

void USGCounterComponentOrbs::RemoveOrbs(const float Amount)
{
	OrbCount -= Amount;
}

bool USGCounterComponentOrbs::CanRemoveOrbs(const float Amount) const
{
	return OrbCount - Amount >= 0.0f;
}

void USGCounterComponentOrbs::ProcessPickup(AActor* Pickup)
{	
	if (ASGPickUpOrbs* Orb = Cast<ASGPickUpOrbs>(Pickup))
	{
		AddOrbs(Orb->GetPickupValue());
		Orb->OnPickup();	
	}
}
