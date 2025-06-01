// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BT_Tasks/BTTask_Patrol.h"

#include "Enemies/AI/SGAIControllerEnemy.h"

UBTTask_Patrol::UBTTask_Patrol()
{
	NodeName = "Start Patrolling";
}

EBTNodeResult::Type UBTTask_Patrol::ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory)
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


