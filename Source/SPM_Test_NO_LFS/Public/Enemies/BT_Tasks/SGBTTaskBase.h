
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "SGBTTaskBase.generated.h"

class ASGAIControllerEnemy;
class ASGEnemyCharacter;

UCLASS()
class SPM_TEST_NO_LFS_API USGBTTaskBase : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	USGBTTaskBase();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere)
	ASGAIControllerEnemy* AIController = nullptr;

	UPROPERTY(EditAnywhere)
	ASGEnemyCharacter* ControlledEnemy = nullptr;

	bool bIsInitialValuesSet = false;
};
