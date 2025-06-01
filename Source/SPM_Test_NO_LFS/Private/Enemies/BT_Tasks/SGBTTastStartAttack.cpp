// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BT_Tasks/SGBTTastStartAttack.h"

#include "Enemies/AI/SGAIControllerEnemy.h"
#include "Enemies/Characters/SGEnemyCharacter.h"
#include "Enemies/Components/SGEnemyAttackComponentBase.h"

USGBTTastStartAttack::USGBTTastStartAttack()
{
	NodeName = TEXT("Start Attack");
}

EBTNodeResult::Type USGBTTastStartAttack::ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory)
{
	Super::ExecuteTask(Comp, NodeMemory);

	if (ControlledEnemy && AIController)
	{
		ControlledEnemy->GetAttackComponent()->StartAttack(AIController->GetAttackTarget());
		
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}


