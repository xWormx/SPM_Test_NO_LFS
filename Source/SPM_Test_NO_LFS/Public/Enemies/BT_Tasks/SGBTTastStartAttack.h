#pragma once

#include "CoreMinimal.h"
#include "SGBTTaskBase.h"
#include "SGBTTastStartAttack.generated.h"

UCLASS()
class SPM_TEST_NO_LFS_API USGBTTastStartAttack : public USGBTTaskBase
{
	GENERATED_BODY()

public:
	USGBTTastStartAttack();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory) override;
};
