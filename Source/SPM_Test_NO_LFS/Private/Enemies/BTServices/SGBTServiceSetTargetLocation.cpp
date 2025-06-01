


#include "Enemies/BTServices/SGBTServiceSetTargetLocation.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Enemies/Characters/SGEnemyCharacter.h"

USGBTServiceSetTargetLocation::USGBTServiceSetTargetLocation()
{
	NodeName = TEXT("Set Target Location");
}

void USGBTServiceSetTargetLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (AIController && ControlledEnemy)
	{
		FVector AttackTargetLocation = AIController->GetAttackTarget()->GetActorLocation();
		FVector TargetLocation = FVector(AttackTargetLocation.X, AttackTargetLocation.Y,
			ControlledEnemy->GetActorLocation().Z - 10.f);
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), TargetLocation);
	}
}
