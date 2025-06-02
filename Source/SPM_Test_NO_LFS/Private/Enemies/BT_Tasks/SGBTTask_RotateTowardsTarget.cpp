// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BT_Tasks/SGBTTask_RotateTowardsTarget.h"

#include "Enemies/AI/SGAIControllerEnemy.h"
#include "Enemies/Characters/SGEnemyCharacter.h"

USGBTTask_RotateTowardsTarget::USGBTTask_RotateTowardsTarget()
{
	NodeName = TEXT("Rotate Towards Target");
}

EBTNodeResult::Type USGBTTask_RotateTowardsTarget::ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory)
{
	Super::ExecuteTask(Comp, NodeMemory);

	ASGAIControllerEnemy* AIController = Cast<ASGAIControllerEnemy>(Comp.GetAIOwner());

	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	ASGEnemyCharacter* ControlledEnemy = Cast<ASGEnemyCharacter>(AIController->GetPawn());

	if (!ControlledEnemy)
	{
		return EBTNodeResult::Failed;
	}

	AIController->RotateTowardsTarget();
	return EBTNodeResult::Succeeded;
}
