// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BTServices/SGBTServiceResetPatrolState.h"

#include "BehaviorTree/BlackboardComponent.h"

USGBTServiceResetPatrolState::USGBTServiceResetPatrolState()
{
	NodeName = TEXT("Reset Patrol State");
}

void USGBTServiceResetPatrolState::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (AIController && ControlledEnemy)
	{
		//GetWorld()->GetTimerManager().ClearTimer(PatrolDelayTimerHandle);
		//GetWorld()->GetTimerManager().ClearAllTimersForObject(AIController);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), false);
	}
}
