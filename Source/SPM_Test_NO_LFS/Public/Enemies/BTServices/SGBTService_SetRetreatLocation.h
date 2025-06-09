

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "SGBTService_SetRetreatLocation.generated.h"

UCLASS()
class SPM_TEST_NO_LFS_API USGBTService_SetRetreatLocation : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	USGBTService_SetRetreatLocation();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
