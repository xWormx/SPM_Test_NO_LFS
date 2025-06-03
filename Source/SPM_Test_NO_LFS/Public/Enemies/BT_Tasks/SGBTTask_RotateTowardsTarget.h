
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SGBTTask_RotateTowardsTarget.generated.h"

UCLASS()
class SPM_TEST_NO_LFS_API USGBTTask_RotateTowardsTarget : public UBTTaskNode
{
	GENERATED_BODY()

public:
	USGBTTask_RotateTowardsTarget();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory) override;
};
