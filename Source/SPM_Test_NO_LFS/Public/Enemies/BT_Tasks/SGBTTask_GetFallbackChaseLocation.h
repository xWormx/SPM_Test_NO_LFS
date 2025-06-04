

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "SGBTTask_GetFallbackChaseLocation.generated.h"


UCLASS()
class SPM_TEST_NO_LFS_API USGBTTask_GetFallbackChaseLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	USGBTTask_GetFallbackChaseLocation();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory) override;
};
