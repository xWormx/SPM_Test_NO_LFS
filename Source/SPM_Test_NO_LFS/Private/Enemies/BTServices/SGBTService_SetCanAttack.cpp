// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BTServices/SGBTService_SetCanAttack.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Enemies/AI/SGAIControllerEnemy.h"

USGBTService_SetCanAttack::USGBTService_SetCanAttack()
{
	NodeName = TEXT("Set Can Attack Target");
}

void USGBTService_SetCanAttack::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ASGAIControllerEnemy* AIController = Cast<ASGAIControllerEnemy>(OwnerComp.GetAIOwner());

	if (!AIController)
	{
		return;
	}
	
	const bool bCanAttackTarget = AIController->CanAttackTarget();

	AIController->GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), bCanAttackTarget);
}
