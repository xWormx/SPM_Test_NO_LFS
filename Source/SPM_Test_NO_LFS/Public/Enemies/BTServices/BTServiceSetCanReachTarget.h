
#pragma once

#include "CoreMinimal.h"
#include "SGBTServiceBase.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTServiceSetCanReachTarget.generated.h"


UCLASS()
class SPM_TEST_NO_LFS_API UBTServiceSetCanReachTarget : public USGBTServiceBase
{
	GENERATED_BODY()

public:
	UBTServiceSetCanReachTarget();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
