#pragma once

#include "CoreMinimal.h"
#include "SGCounterComponent.h"
#include "SGCounterComponentOrbs.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SPM_TEST_NO_LFS_API USGCounterComponentOrbs : public USGCounterComponent
{
	GENERATED_BODY()

public:
	USGCounterComponentOrbs();

	UFUNCTION(BlueprintPure, Category = "UFunction - Counter Component")
	float GetOrbCount() const;

protected:
	virtual void BeginPlay() override;
	
	virtual void ProcessPickup(AActor* Pickup) override;	
};
