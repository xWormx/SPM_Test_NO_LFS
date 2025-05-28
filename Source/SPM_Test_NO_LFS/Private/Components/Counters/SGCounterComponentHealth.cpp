#include "Components/Counters/SGCounterComponentHealth.h"

#include "SPM_Test_NO_LFS.h"
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
		HealthComponent->OnHurt.AddDynamic(this, &USGCounterComponentHealth::StartHealthBuffer);
	}

	if (USGUpgradeSubsystem* UpgradeSystem = GetOwner()->GetGameInstance()->GetSubsystem<USGUpgradeSubsystem>())
	{
		FName Category = TEXT("Player");
		FName MaxHealth = TEXT("MaxHealth");
		FName HealthBuffCapacity = TEXT("HealthBufferCapacity");
		UpgradeSystem->BindAttribute(HealthComponent, MaxHealth, MaxHealth, Category);
		UpgradeSystem->BindDependentAttribute(HealthComponent, TEXT("CurrentHealth"), false, HealthComponent, MaxHealth);
		
		UpgradeSystem->BindAttribute(this, HealthBuffCapacity, TEXT("MaxHealthBufferCapacity"), Category);
		UpgradeSystem->BindDependentAttribute(this, TEXT("HealthBuffer"), false, this, HealthBuffCapacity);
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
		HealthBuffer += CurrentHealth + Amount - MaxHealth; //Överför överskott till shield
		HealthComponent->Heal(MaxHealth);
		StartHealthBufferTimer();
	}
	else
	{
		HealthComponent->Heal(Amount);
	}	
}

void USGCounterComponentHealth::StartHealthBuffer(float NewHealth)
{
	if (HealthBuffer > HealthBufferCapacity)
	{
		HealthBuffer = HealthBufferCapacity;
	}
	if (HealthBufferTimerInterval <= 0.f)
	{
		float Damage = HealthComponent->GetMaxHealth() - NewHealth;
		DecreaseHealthBuffer(HealthBuffer - Damage < 0.f ? HealthBuffer : Damage);
	}
}

void USGCounterComponentHealth::StartHealthBufferTimer()
{
	if (HealthBuffer > HealthBufferCapacity)
	{
		HealthBuffer = HealthBufferCapacity;
	}
	if (HealthBufferTimerInterval <= 0.f)
	{
		DecreaseHealthBuffer(HealthBuffer);
	}

	if (GetWorld()->GetTimerManager().IsTimerActive(HealthBufferTimerHandle))
	{
		return;
	}
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(this, &USGCounterComponentHealth::DecreaseHealthBuffer, HealthBufferDecreaseRate);
	GetWorld()->GetTimerManager().SetTimer(HealthBufferTimerHandle, TimerDelegate, HealthBufferTimerInterval, true);
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
	else if (HealthBufferTimerInterval > 0.f)
	{
		HealthBuffer -= Amount;
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
