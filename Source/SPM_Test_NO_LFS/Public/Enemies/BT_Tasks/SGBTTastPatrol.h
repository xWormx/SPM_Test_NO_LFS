
#pragma once

#include "CoreMinimal.h"
#include "SGBTTaskBase.h"
#include "SGBTTastPatrol.generated.h"

UCLASS()
class SPM_TEST_NO_LFS_API USGBTTastPatrol : public USGBTTaskBase
{
	GENERATED_BODY()

public:
	USGBTTastPatrol();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
