
#pragma once

#include "CoreMinimal.h"
#include "SGBTTaskBase.h"
#include "SGBTTaskClearBlackboardValue.generated.h"

UCLASS()
class SPM_TEST_NO_LFS_API USGBTTaskClearBlackboardValue : public USGBTTaskBase
{
	GENERATED_BODY()

public:
	USGBTTaskClearBlackboardValue();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
