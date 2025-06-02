

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "SGBTService_SetCanAttack.generated.h"


UCLASS()
class SPM_TEST_NO_LFS_API USGBTService_SetCanAttack : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	USGBTService_SetCanAttack();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
