// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BT_Tasks/SGBTTaskBase.h"

#include "Enemies/AI/SGAIControllerEnemy.h"
#include "Enemies/Characters/SGEnemyCharacter.h"

USGBTTaskBase::USGBTTaskBase()
{
	NodeName = TEXT("BT Task Base");
}

EBTNodeResult::Type USGBTTaskBase::ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory)
{
	Super::ExecuteTask(Comp, NodeMemory);

	if (!bIsInitialValuesSet)
	{
		AIController = Cast<ASGAIControllerEnemy>(Comp.GetAIOwner());

		if (AIController)
		{
			ControlledEnemy = Cast<ASGEnemyCharacter>(AIController->GetPawn());

			if (ControlledEnemy)
			{
				bIsInitialValuesSet = true;
			}
		}
	}

	if (bIsInitialValuesSet)
	{
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
