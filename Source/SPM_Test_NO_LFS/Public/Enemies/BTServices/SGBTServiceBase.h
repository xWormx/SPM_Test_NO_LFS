

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "Enemies/AI/SGAIControllerEnemy.h"
#include "SGBTServiceBase.generated.h"

class ASGAIController;
class ASGEnemyCharacter;

UCLASS()
class SPM_TEST_NO_LFS_API USGBTServiceBase : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	USGBTServiceBase();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere)
	ASGAIControllerEnemy* AIController = nullptr;

	UPROPERTY(EditAnywhere)
	ASGEnemyCharacter* ControlledEnemy = nullptr;

	bool bIsInitialValuesSet = false;

	FTimerHandle PatrolDelayTimerHandle;
};
