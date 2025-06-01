// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BT_Tasks/SGBTTask_StartAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemies/AI/SGAIControllerEnemy.h"
#include "Enemies/Characters/SGEnemyCharacter.h"
#include "Enemies/Components/SGEnemyAttackComponentBase.h"

USGBTTask_StartAttack::USGBTTask_StartAttack()
{
	NodeName = TEXT("Start Attacking");
}

EBTNodeResult::Type USGBTTask_StartAttack::ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory)
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

	AActor* AttackTarget = Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject("AttackTarget"));


	if (!AttackTarget)
	{
		return EBTNodeResult::Failed;
	}

	ControlledEnemy->GetAttackComponent()->StartAttack(AttackTarget);
	
	return EBTNodeResult::Succeeded;
}
