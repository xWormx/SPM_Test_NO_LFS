// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BT_Tasks/BTDecoratorTest.h"

#include "AIController.h"

#include "Enemies/AI/SGAIControllerEnemy.h"

UBTDecoratorTest::UBTDecoratorTest()
{
	NodeName = TEXT("BT Decorator Test");
}

bool UBTDecoratorTest::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	ASGAIControllerEnemy* AIController = Cast<ASGAIControllerEnemy>(OwnerComp.GetAIOwner());
	if (!AIController)
	{
		return false;
	}

	return AIController->CanReachTarget(AIController->GetAttackTarget());
}
