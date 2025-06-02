// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BT_Tasks/SGBTTask_ResetShouldPatrol.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Enemies/AI/SGAIControllerEnemy.h"

USGBTTask_ResetShouldPatrol::USGBTTask_ResetShouldPatrol()
{
	NodeName = TEXT("Reset Should Patrol");
}

EBTNodeResult::Type USGBTTask_ResetShouldPatrol::ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory)
{
	Super::ExecuteTask(Comp, NodeMemory);

	ASGAIControllerEnemy* AIController = Cast<ASGAIControllerEnemy>(Comp.GetAIOwner());

	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	if (GetWorld()->GetTimerManager().IsTimerActive(AIController->PatrolDelayTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(AIController->PatrolDelayTimer);
	}

	AIController->bShouldPatrol = false;

	AIController->GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), false);

	return EBTNodeResult::Succeeded;
}
