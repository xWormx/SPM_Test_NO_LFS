#pragma once

#include "CoreMinimal.h"
#include "SGCounterComponent.h"
#include "SGCounterComponentAmmo.generated.h"

class ASGGun;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPickedUpAmmo, int32, AmmoCount, ASGGun*, Gun);

class ASGPlayerCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SPM_TEST_NO_LFS_API USGCounterComponentAmmo : public USGCounterComponent
{
	GENERATED_BODY()

public:
	USGCounterComponentAmmo();

	FOnPickedUpAmmo OnPickedUpAmmo;
protected:
	virtual void BeginPlay() override;
	virtual void ProcessPickup(AActor* Pickup) override;

private:
	TWeakObjectPtr<ASGPlayerCharacter> OwnerPlayer;
};