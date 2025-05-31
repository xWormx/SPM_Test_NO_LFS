// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BTServices/SGBTServiceBase.h"
#include "Enemies/AI/SGAIControllerEnemy.h"
#include "Enemies/Characters/SGEnemyCharacter.h"


USGBTServiceBase::USGBTServiceBase()
{
	NodeName = TEXT("BT Service Base");
}

void USGBTServiceBase::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (!bIsInitialValuesSet)
	{
		AIController = Cast<ASGAIControllerEnemy>(OwnerComp.GetAIOwner());
		
		if (AIController)
		{
			ControlledEnemy = Cast<ASGEnemyCharacter>(AIController->GetPawn());
			
			if (ControlledEnemy)
			{
				bIsInitialValuesSet = true;
			}
		}
	}
}
