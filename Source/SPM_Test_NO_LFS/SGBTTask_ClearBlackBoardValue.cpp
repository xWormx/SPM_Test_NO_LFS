// Fill out your copyright notice in the Description page of Project Settings.


#include "SGBTTask_ClearBlackBoardValue.h"

#include "BehaviorTree/BlackboardComponent.h"

USGBTTask_ClearBlackBoardValue::USGBTTask_ClearBlackBoardValue()
{
	NodeName = TEXT("Clear Blackboard Value");
}

EBTNodeResult::Type USGBTTask_ClearBlackBoardValue::ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory)
{
	Super::ExecuteTask(Comp, NodeMemory);

	Comp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());

	return EBTNodeResult::Succeeded;
	
}

