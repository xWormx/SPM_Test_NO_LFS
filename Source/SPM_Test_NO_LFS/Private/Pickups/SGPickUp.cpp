#include "Pickups/SGPickUp.h"

#include "Utils/SGUtilObjectPoolManager.h"
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
	GetGameInstance()->GetSubsystem<USGObjectPoolSubsystem>()->ReturnObjectToPool(this);
}

void ASGPickUp::SetPickUpValue(float Value)
{
	PickUpValue = Value;
}