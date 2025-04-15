#include "SGCounterComponentHealth.h"
#include "SGPickUpHealthPack.h"


USGCounterComponentHealth::USGCounterComponentHealth()
{	
	PrimaryComponentTick.bCanEverTick = true;	
}

float USGCounterComponentHealth::GetCurrentHealth() const
{
	return CurrentHealth;
}

float USGCounterComponentHealth::GetBufferedHealth() const
{
	return HealthBuffer;
}

void USGCounterComponentHealth::IncreaseHealth(const float Amount)
{
	if (CurrentHealth == MaxHealth)
	{
		HealthBuffer += Amount;
	}
	else if (CurrentHealth + Amount > MaxHealth)
	{
		HealthBuffer += (CurrentHealth + Amount) - MaxHealth;
		CurrentHealth = MaxHealth;
	}
	else
	{
		CurrentHealth += Amount;
	}
}

void USGCounterComponentHealth::DecreaseHealth(const float Amount)
{
	if (HealthBuffer > 0)
	{
		HealthBuffer -= Amount;
		if (HealthBuffer < 0)
		{
			CurrentHealth += HealthBuffer; //HealthBuffer har negativt värde
			HealthBuffer = 0;
		}
	}
	else
	{
		CurrentHealth -= Amount;
	}

	if (CurrentHealth <= 0)
	{
		CurrentHealth = 0;
		OnNoHealth.Broadcast(0);
	}
}

void USGCounterComponentHealth::ProcessPickup(AActor* Pickup)
{
	if (ASGPickUpHealthPack* HealthPacket = Cast<ASGPickUpHealthPack>(Pickup))
	{
		IncreaseHealth(HealthPacket->GetPickupValue());
		LogCounter();

		HealthPacket->OnPickup();
	}
}

void USGCounterComponentHealth::LogCounter()
{
	UE_LOG(LogTemp, Warning, TEXT("Current Health: %f, Buffered Health: %f"), CurrentHealth, HealthBuffer);
}