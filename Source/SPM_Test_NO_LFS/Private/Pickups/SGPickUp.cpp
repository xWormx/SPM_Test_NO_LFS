#include "Pickups/SGPickUp.h"

#include "Components/SphereComponent.h"
#include "Utils/SGObjectPoolSubsystem.h"

ASGPickUp::ASGPickUp()
{
	PrimaryActorTick.bCanEverTick = true;
	
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PickUpArea"));
	CollisionSphere->SetSphereRadius(100.0f);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionSphere->SetCollisionResponseToAllChannels(ECR_Overlap);
	RootComponent = CollisionSphere;
}

void ASGPickUp::OnPickup_Implementation()
{
	GetGameInstance()->GetSubsystem<USGObjectPoolSubsystem>()->ReturnObjectToPool(this);
}

float ASGPickUp::GetPickupValue() const
{
	return PickUpValue;
}

void ASGPickUp::SetPickUpValue(const float Value)
{
	PickUpValue = Value;
}
