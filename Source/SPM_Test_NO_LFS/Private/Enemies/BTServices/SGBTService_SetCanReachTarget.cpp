

#include "Enemies/BTServices/SGBTService_SetCanReachTarget.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Enemies/AI/SGAIControllerEnemy.h"


USGBTService_SetCanReachTarget::USGBTService_SetCanReachTarget()
{
	NodeName = TEXT("Set Can Reach Target");
}

void USGBTService_SetCanReachTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ASGAIControllerEnemy* AIController = Cast<ASGAIControllerEnemy>(OwnerComp.GetAIOwner());

	if (!AIController)
	{
		return;
	}

	AActor* TargetActor = Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject("AttackTarget"));

	if (!TargetActor)
	{
		return;
	}
	const bool bCanReachTarget = AIController->CanReachTarget();

	AIController->GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), bCanReachTarget);
}
