#include "SGCounterComponentOrbs.h"
#include "SGPickUpOrbs.h"


USGCounterComponentOrbs::USGCounterComponentOrbs()
{	
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

void USGCounterComponentOrbs::ProcessPickup(AActor* Pickup)
{	
	if (ASGPickUpOrbs* Orb = Cast<ASGPickUpOrbs>(Pickup))
	{
		AddOrbs(Orb->GetPickupValue());
		Orb->OnPickup();	
	}
}
