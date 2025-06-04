#include "Pickups/SGPickUp.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
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

// Called when the game starts or when spawned
void ASGPickUp::BeginPlay()
{
	Super::BeginPlay();	
}

void ASGPickUp::OnPickup_Implementation()
{
	if (PickUpSound) UGameplayStatics::PlaySound2D(this, PickUpSound);
	
	GetGameInstance()->GetSubsystem<USGObjectPoolSubsystem>()->ReturnObjectToPool(this);
}

float ASGPickUp::GetPickupValue() const
{
	return PickUpValue;
}

void ASGPickUp::SetPickUpValue(float Value)
{
	PickUpValue = Value;
}
