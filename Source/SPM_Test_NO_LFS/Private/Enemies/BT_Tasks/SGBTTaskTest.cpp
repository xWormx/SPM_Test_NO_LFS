// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BT_Tasks/SGBTTaskTest.h"

#include "Enemies/AI/SGAIControllerEnemy.h"

USGBTTaskTest::USGBTTaskTest()
{
	NodeName = TEXT("BT Task Test");
}

EBTNodeResult::Type USGBTTaskTest::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::ExecuteTask(OwnerComp, NodeMemory);

	
}
