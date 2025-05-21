#include "Components/Counters/SGCounterComponentAmmo.h"

#include "jola6902_GunsComponent.h"
#include "Gear/Weapons/SGGun.h"
#include "Pickups/SGPickUpAmmo.h"
#include "Player/SGPlayerCharacter.h"

USGCounterComponentAmmo::USGCounterComponentAmmo()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USGCounterComponentAmmo::BeginPlay()
{
	Super::BeginPlay();
	OwnerPlayer = Cast<ASGPlayerCharacter>(GetOwner());
}

void USGCounterComponentAmmo::ProcessPickup(AActor* Pickup)
{
	ASGPickUpAmmo* Ammo = Cast<ASGPickUpAmmo>(Pickup);
	if (!Ammo || !OwnerPlayer.IsValid())
	{
		return;
	}

	for (ASGGun* Gun : OwnerPlayer->GunsComponent->GetGuns())
	{
		if (!Ammo->IsCompatibleWithGun(Gun->GetClass()))
		{
			continue;
		}

		Gun->AddAmmo(Ammo->GetPickupValue());
		break; //Ta bort denna om flera vapen kan använda samma ammo
	}

	Ammo->OnPickup();
}