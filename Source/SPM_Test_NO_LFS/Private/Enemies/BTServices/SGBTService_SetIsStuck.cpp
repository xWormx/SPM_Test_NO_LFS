// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BTServices/SGBTService_SetIsStuck.h"

#include "SPM_Test_NO_LFS.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemies/AI/SGAIControllerEnemy.h"

USGBTService_SetIsStuck::USGBTService_SetIsStuck()
{
	NodeName = TEXT("Set Is Stuck");
}

void USGBTService_SetIsStuck::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ASGAIControllerEnemy* AIController = Cast<ASGAIControllerEnemy>(OwnerComp.GetAIOwner());

	if (!AIController)
	{
		BASIR_LOG(Error, TEXT("Set Is Stuck, AIController is Null"));
		return;
	}

	const bool bIsStuck = AIController->IsStuck();

	if (bIsStuck)
	{
		AIController->GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), true);
		return;
	}

	AIController->GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), false);
}
