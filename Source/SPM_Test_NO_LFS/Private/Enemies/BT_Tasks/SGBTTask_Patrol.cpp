// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BT_Tasks/SGBTTask_Patrol.h"

#include "Enemies/AI/SGAIControllerEnemy.h"

USGBTTask_Patrol::USGBTTask_Patrol()
{
	NodeName = "Start Patrolling";
}

EBTNodeResult::Type USGBTTask_Patrol::ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory)
{
	Super::ExecuteTask(Comp, NodeMemory);

	ASGAIControllerEnemy* AICOntroller = Cast<ASGAIControllerEnemy>(Comp.GetAIOwner());

	if (!AICOntroller)
	{
		return EBTNodeResult::Failed;
	}

	AICOntroller->Patrol();

	return EBTNodeResult::Succeeded;
}


