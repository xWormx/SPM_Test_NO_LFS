


#include "Enemies/BTServices/SGBTService_SetCanSeeTarget.h"

#include "SPM_Test_NO_LFS.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemies/AI/SGAIControllerEnemy.h"
#include "Enemies/AI/SGAIControllerEnemyFlying_2.h"

USGBTService_SetCanSeeTarget::USGBTService_SetCanSeeTarget()
{
	NodeName = TEXT("Set Can See Target");
}

void USGBTService_SetCanSeeTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ASGAIControllerEnemyFlying_2* AIController = Cast<ASGAIControllerEnemyFlying_2>(OwnerComp.GetAIOwner());

	if (!AIController)
	{
		BASIR_LOG(Error, TEXT("Set Can See Target, AIController is Null"));
		return;
	}

	AActor* AttackTarget = Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject("AttackTarget"));
	
	if (!AttackTarget)
	{
		BASIR_LOG(Error, TEXT("Set Can See Target, AttackTarget is Null"));
		return;
	}

	if (AIController->LineOfSightTo(AttackTarget))
	{
		AIController->GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), true);
	}
	else
	{
		AIController->GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), false);
	}
}
