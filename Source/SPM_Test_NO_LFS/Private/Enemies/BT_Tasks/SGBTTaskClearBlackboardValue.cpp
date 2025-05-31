// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BT_Tasks/SGBTTaskClearBlackboardValue.h"

#include "BehaviorTree/BlackboardComponent.h"

USGBTTaskClearBlackboardValue::USGBTTaskClearBlackboardValue()
{
	NodeName = TEXT("Clear Blackboard Value");
}

EBTNodeResult::Type USGBTTaskClearBlackboardValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (AIController && ControlledEnemy)
	{
		OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
