#include "Components/SGHealthComponent.h"

USGHealthComponent::USGHealthComponent()
{
	MaxHealth = 100;
	CurrentHealth = MaxHealth;
}

void USGHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &USGHealthComponent::DamageTaken);	
}

void USGHealthComponent::DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* Instigator, AActor* DamageCauser)
{
	if (Damage <= 0.f)
	{
		return;
	}
	
	CurrentHealth -= Damage;	
	OnHurt.Broadcast(CurrentHealth);
	
	if (CurrentHealth <= 0.f)
	{
		CurrentHealth = 0;
		OnNoHealth.Broadcast(CurrentHealth);		
	}
}

void USGHealthComponent::Heal(float HealAmount)
{
	if (HealAmount <= 0.f || CurrentHealth == MaxHealth)
	{
		return;
	}
	
	CurrentHealth = CurrentHealth + HealAmount >= MaxHealth ? MaxHealth : CurrentHealth + HealAmount;	
}

float USGHealthComponent::GetCurrentHealth() const
{
	return CurrentHealth;
}

float USGHealthComponent::GetMaxHealth() const
{
	return MaxHealth;
}

void USGHealthComponent::SetCurrentHealth(float NewHealth)
{
	CurrentHealth = NewHealth;
}


