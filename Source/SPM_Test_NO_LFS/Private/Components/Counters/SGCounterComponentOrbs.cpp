#include "Components/Counters/SGCounterComponentOrbs.h"

#include "Core/SGUpgradeGuardSubsystem.h"
#include "Core/SGUpgradeSubsystem.h"
#include "Pickups/SGPickUpOrbs.h"

USGCounterComponentOrbs::USGCounterComponentOrbs()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USGCounterComponentOrbs::BeginPlay()
{
	Super::BeginPlay();

	UpgradeGuard = GetOwner()->GetGameInstance()->GetSubsystem<USGUpgradeGuardSubsystem>();
	USGUpgradeSubsystem* UpgradeSubsystem = GetOwner()->GetGameInstance()->GetSubsystem<USGUpgradeSubsystem>();
	if (!UpgradeGuard.IsValid() || !UpgradeSubsystem)
	{
		return;
	}	
	UpgradeSubsystem->OnUpgradeCost.AddDynamic(UpgradeGuard.Get(), &USGUpgradeGuardSubsystem::RemoveFromCount);
}

void USGCounterComponentOrbs::ProcessPickup(AActor* Pickup)
{
	ASGPickUpOrbs* Orb = Cast<ASGPickUpOrbs>(Pickup);
	if (!Orb || !UpgradeGuard.IsValid())
	{
		return;
	}		
	UpgradeGuard.Get()->AddToCount(Orb->GetPickupValue());
	Orb->OnPickup();
}
