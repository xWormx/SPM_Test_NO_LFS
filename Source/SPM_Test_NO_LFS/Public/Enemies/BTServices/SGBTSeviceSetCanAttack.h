
#pragma once

#include "CoreMinimal.h"
#include "Enemies/BTServices/SGBTServiceBase.h"
#include "SGBTSeviceSetCanAttack.generated.h"


UCLASS()
class SPM_TEST_NO_LFS_API USGBTSeviceSetCanAttack : public USGBTServiceBase
{
	GENERATED_BODY()

public:
	USGBTSeviceSetCanAttack();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
