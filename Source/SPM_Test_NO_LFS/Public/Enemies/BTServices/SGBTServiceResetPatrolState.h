

#pragma once

#include "CoreMinimal.h"
#include "SGBTServiceBase.h"
#include "SGBTServiceResetPatrolState.generated.h"

UCLASS()
class SPM_TEST_NO_LFS_API USGBTServiceResetPatrolState : public USGBTServiceBase
{
	GENERATED_BODY()

public:
	USGBTServiceResetPatrolState();


protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
