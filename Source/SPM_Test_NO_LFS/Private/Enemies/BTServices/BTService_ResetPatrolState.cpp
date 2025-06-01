// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BTServices/BTService_ResetPatrolState.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Enemies/AI/SGAIControllerEnemy.h"

UBTService_ResetPatrolState::UBTService_ResetPatrolState()
{
	NodeName = "Reset Patrol State";
}

void UBTService_ResetPatrolState::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ASGAIControllerEnemy* AIController = Cast<ASGAIControllerEnemy>(OwnerComp.GetAIOwner());

	if (!AIController)
	{
		return;
	}

	if (GetWorld()->GetTimerManager().IsTimerActive(AIController->PatrolDelayTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(AIController->PatrolDelayTimer);
	}

	AIController->bShouldPatrol = false;

	AIController->GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), false);
}
