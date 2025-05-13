#pragma once

#include "CoreMinimal.h"
#include "SGCounterComponent.h"
#include "SGCounterComponentAmmo.generated.h"

class ASGPlayerCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SPM_TEST_NO_LFS_API USGCounterComponentAmmo : public USGCounterComponent
{
	GENERATED_BODY()

public:
	USGCounterComponentAmmo();

protected:
	virtual void BeginPlay() override;
	virtual void ProcessPickup(AActor* Pickup) override;

private:
	TWeakObjectPtr<ASGPlayerCharacter> OwnerPlayer;
};