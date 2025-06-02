

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_SetShouldRetreat.generated.h"


UCLASS()
class SPM_TEST_NO_LFS_API UBTService_SetShouldRetreat : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_SetShouldRetreat();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
