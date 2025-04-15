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
		LogCounter(); 

		Orb->OnPickup();	
	}
}

void USGCounterComponentOrbs::LogCounter()
{
	UE_LOG(LogTemp, Warning, TEXT("USGCounterComponentOrbs: Orb Count: %f"), OrbCount);
}