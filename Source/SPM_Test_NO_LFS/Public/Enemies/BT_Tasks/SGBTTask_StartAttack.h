

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SGBTTask_StartAttack.generated.h"


UCLASS()
class SPM_TEST_NO_LFS_API USGBTTask_StartAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	USGBTTask_StartAttack();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory) override;
};
