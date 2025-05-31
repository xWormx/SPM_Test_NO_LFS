

#pragma once

#include "CoreMinimal.h"
#include "SGBTServiceBase.h"
#include "SGBTServiceSetTargetLocation.generated.h"

UCLASS()
class SPM_TEST_NO_LFS_API USGBTServiceSetTargetLocation : public USGBTServiceBase
{
	GENERATED_BODY()

public:
	USGBTServiceSetTargetLocation();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
