#pragma once

#include "CoreMinimal.h"
#include "SGCounterComponent.h"
#include "SGCounterComponentOrbs.generated.h"

class USGUpgradeGuardSubsystem;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SPM_TEST_NO_LFS_API USGCounterComponentOrbs : public USGCounterComponent
{
	GENERATED_BODY()

public:
	USGCounterComponentOrbs();
	
protected:
	virtual void BeginPlay() override;
	
	virtual void ProcessPickup(AActor* Pickup) override;

private:
	TWeakObjectPtr<USGUpgradeGuardSubsystem> UpgradeGuard;
};
