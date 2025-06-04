// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BT_Tasks/SGBTTast_ClearBlackboardValue.h"

#include "SPM_Test_NO_LFS.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemies/AI/SGAIControllerEnemy.h"

USGBTTast_ClearBlackboardValue::USGBTTast_ClearBlackboardValue()
{
	NodeName = TEXT("Clear Blackboard Value");
}

EBTNodeResult::Type USGBTTast_ClearBlackboardValue::ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory)
{
	Super::ExecuteTask(Comp, NodeMemory);

	ASGAIControllerEnemy* AIController = Cast<ASGAIControllerEnemy>(Comp.GetAIOwner());

	if (!AIController)
	{
		BASIR_LOG(Error, TEXT("Clear BB Value, AIController is Null"));
		return EBTNodeResult::Failed;
	}

	AIController->GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
	return EBTNodeResult::Succeeded;
}
