#include "Components/Counters/SGCounterComponentOrbs.h"

#include "Core/SGUpgradeGuardGameInstance.h"
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
		USGUpgradeGuardGameInstance* UpgradeGuardInstance = GetOwner()->GetGameInstance<USGUpgradeGuardGameInstance>();
		if (!UpgradeGuardInstance)
		{
			return;
		}
		//TODO: Se över lösning. Känns som att jag gör något olagligt här💀
		UpgradeSubsystem->OnUpgradeCost.AddDynamic(UpgradeGuardInstance, &USGUpgradeGuardGameInstance::RemoveFromCount);
	}
}

float USGCounterComponentOrbs::GetOrbCount() const
{
	const USGUpgradeGuardGameInstance* UpgradeGuardInstance = GetOwner()->GetGameInstance<USGUpgradeGuardGameInstance>();
	if (!UpgradeGuardInstance)
	{
		return 0.0f;
	}
	return UpgradeGuardInstance->GetCount(); //TODO: Vid ändring av widget så lär den hämta från UpgradeGuardInstance istället för att hämta här. 	
}

void USGCounterComponentOrbs::ProcessPickup(AActor* Pickup)
{
	ASGPickUpOrbs* Orb = Cast<ASGPickUpOrbs>(Pickup);
	if (!Orb)
	{
		return;
	}
	//TODO: Kan vara läge att casha USGUpgradeGuardGameInstance
	USGUpgradeGuardGameInstance* UpgradeGuardInstance = GetOwner()->GetGameInstance<USGUpgradeGuardGameInstance>();
	if (!UpgradeGuardInstance)
	{
		return;
	}
	UpgradeGuardInstance->AddToCount(Orb->GetPickupValue());
	Orb->OnPickup();
}
