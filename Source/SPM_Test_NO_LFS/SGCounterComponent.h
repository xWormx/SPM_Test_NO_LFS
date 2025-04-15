#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SGCounterComponent.generated.h"


UCLASS(Abstract, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SPM_TEST_NO_LFS_API USGCounterComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USGCounterComponent();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

	virtual void ProcessPickup(AActor* Pickup) PURE_VIRTUAL(UBaseCounterComponent::ProcessPickup, );

	//TODO: Byt ut så counter syns i en widget
	virtual void LogCounter() PURE_VIRTUAL(UBaseCounterComponent::LogCounter, );
	
};