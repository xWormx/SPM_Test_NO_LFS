#include "Components/Counters/SGCounterComponentHealth.h"

#include "Components/SGHealthComponent.h"
#include "Core/SGUpgradeSubsystem.h"
#include "Pickups/SGPickUpHealthPack.h"

//TODO: Potentiellt värt att ändra så att buffert-delen och FTimern i klassen får jobba på eget håll. Ifall den funktionalitet skulle behövas i andra klasser.

USGCounterComponentHealth::USGCounterComponentHealth(){}

void USGCounterComponentHealth::BeginPlay()
{
	Super::BeginPlay();

	HealthComponent = GetOwner()->FindComponentByClass<USGHealthComponent>();

	// Damage Control - lägger till en Health Component ifall den inte redan finns. 
	if (!HealthComponent)
	{
		HealthComponent = NewObject<USGHealthComponent>(GetOwner(), USGHealthComponent::StaticClass(),TEXT("HealthComponent"));

		if (HealthComponent)
		{
			HealthComponent->RegisterComponent();
			HealthComponent->SetIsReplicated(true);
		}
	}

	if (HealthComponent)
	{
		HealthComponent->OnNoHealth.AddDynamic(this, &USGCounterComponentHealth::UseHealthBuffer);
	}

	if (USGUpgradeSubsystem* UpgradeSystem = GetOwner()->GetGameInstance()->GetSubsystem<USGUpgradeSubsystem>())
	{
		FName Category = TEXT("Player");
		UpgradeSystem->BindAttribute(HealthComponent, TEXT("MaxHealth"), TEXT("MaxHealth"), Category);
		UpgradeSystem->BindAttribute(this, TEXT("HealthBufferCapacity"), TEXT("MaxHealthBufferCapacity"), Category);
	}
}

void USGCounterComponentHealth::IncreaseHealth(const float Amount)
{
	const float CurrentHealth = HealthComponent->GetCurrentHealth();
	const float MaxHealth = HealthComponent->GetMaxHealth();

	if (CurrentHealth == MaxHealth)
	{
		HealthBuffer += Amount;
		StartHealthBufferTimer(); //Startar bara om timern inte är aktiv
	}
	else if (CurrentHealth + Amount > MaxHealth)
	{
		HealthBuffer += (CurrentHealth + Amount) - MaxHealth; //Överför överskott till buffert		
		HealthComponent->Heal(MaxHealth);
	}
	else
	{
		HealthComponent->Heal(Amount);
	}	
}

void USGCounterComponentHealth::UseHealthBuffer(const float Amount)
{
	if (HealthBuffer <= 0)
	{
		return;
	}
	
	const float CurrentHealth = HealthComponent->GetCurrentHealth();
	HealthBuffer -= Amount;
	
	if (HealthBuffer < 0 && CurrentHealth > 0)
	{
		HealthComponent->SetCurrentHealth(CurrentHealth + HealthBuffer); //Överför underskott till HealthComponent
		HealthBuffer = 0;
	}
	else
	{
		HealthComponent->Heal(Amount);
	}
	
}

void USGCounterComponentHealth::ProcessPickup(AActor* Pickup)
{
	if (ASGPickUpHealthPack* HealthPacket = Cast<ASGPickUpHealthPack>(Pickup))
	{
		IncreaseHealth(HealthPacket->GetPickupValue());
		HealthPacket->OnPickup();
	}
}

void USGCounterComponentHealth::StartHealthBufferTimer()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(HealthBufferTimerHandle))
	{
		return;
	}
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(this, &USGCounterComponentHealth::DecreaseHealthBuffer, HealthBufferDecreaseRate);
	GetWorld()->GetTimerManager().SetTimer(HealthBufferTimerHandle, TimerDelegate, HealthBufferDecreaseRate, true);
}

void USGCounterComponentHealth::DecreaseHealthBuffer(const float Amount)
{
	if (HealthBuffer <= 0)
	{
		HealthBuffer = 0;
		GetWorld()->GetTimerManager().ClearTimer(HealthBufferTimerHandle);
	}
	else if (HealthComponent->GetCurrentHealth() < HealthComponent->GetMaxHealth() )
	{
		UseHealthBuffer(Amount);
	}
	else
	{
		UseHealthBuffer(Amount);
	}

	/*if (HealthBuffer > 0)
	{
		UseHealthBuffer(Amount);
	}
	else
	{
		HealthBuffer = 0;
		GetWorld()->GetTimerManager().ClearTimer(HealthBufferTimerHandle);
	}*/
}

float USGCounterComponentHealth::GetCurrentHealth() const
{
	return HealthComponent->GetCurrentHealth();
}

float USGCounterComponentHealth::GetBufferedHealth() const
{
	return HealthBuffer;
}

float USGCounterComponentHealth::GetBufferCapacity() const
{
	return HealthBufferCapacity;
}
