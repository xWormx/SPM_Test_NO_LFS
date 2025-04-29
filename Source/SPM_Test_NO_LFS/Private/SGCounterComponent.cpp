#include "SGCounterComponent.h"


USGCounterComponent::USGCounterComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void USGCounterComponent::BeginPlay()
{
	Super::BeginPlay();
	if (AActor* Owner = GetOwner())
	{
		Owner->OnActorBeginOverlap.AddDynamic(this, &USGCounterComponent::OnOverlapBegin);
	}
}

void USGCounterComponent::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (!OtherActor || OtherActor == GetOwner())
	{
		return;
	}
	ProcessPickup(OtherActor);
}