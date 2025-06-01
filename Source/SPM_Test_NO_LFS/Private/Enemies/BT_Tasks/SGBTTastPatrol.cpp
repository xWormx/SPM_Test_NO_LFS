// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BT_Tasks/SGBTTastPatrol.h"

#include "Enemies/AI/SGAIControllerEnemy.h"

USGBTTastPatrol::USGBTTastPatrol()
{
	NodeName = TEXT("Patrol");
}

EBTNodeResult::Type USGBTTastPatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (AIController)
	{
		AIController->Patrol();
		UE_LOG(LogTemp, Warning, TEXT("Succeeded"));
		return EBTNodeResult::Succeeded;
	}

	if (AIController->IsStuck())
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed"));
	}
	return EBTNodeResult::Failed;
}


