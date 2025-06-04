// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BT_Tasks/SGBTTask_SetBlackboardValue.h"

#include "SPM_Test_NO_LFS.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemies/AI/SGAIControllerEnemy.h"

USGBTTask_SetBlackboardValue::USGBTTask_SetBlackboardValue()
{
	NodeName = TEXT("Set Blackboard Value");
}

EBTNodeResult::Type USGBTTask_SetBlackboardValue::ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory)
{
	Super::ExecuteTask(Comp, NodeMemory);

	ASGAIControllerEnemy* AIController = Cast<ASGAIControllerEnemy>(Comp.GetAIOwner());

	if (!AIController)
	{
		BASIR_LOG(Error, TEXT("Set Blackboard Value, AIController is Null"));
		return EBTNodeResult::Failed;
	}

	UBlackboardComponent* BTComp = Comp.GetBlackboardComponent();

	if (!BTComp)
	{
		BASIR_LOG(Error, TEXT("Set Blackboard Value, BTComp is Null"));
		return EBTNodeResult::Failed;
	}

	BTComp->SetValueAsBool(GetSelectedBlackboardKey(), true);
	return EBTNodeResult::Succeeded;
	
}


