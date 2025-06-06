// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BTServices/SGBTService_SetIsFacingTarget.h"

#include "SPM_Test_NO_LFS.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemies/AI/SGAIControllerEnemy.h"

USGBTService_SetIsFacingTarget::USGBTService_SetIsFacingTarget()
{
	NodeName = TEXT("Set Is Facing Target");
}

void USGBTService_SetIsFacingTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ASGAIControllerEnemy* AIController = Cast<ASGAIControllerEnemy>(OwnerComp.GetAIOwner());

	if (!AIController)
	{
		BASIR_LOG(Error, TEXT("Set Is Facing Target, AIController is Null"));
	}

	const bool bIsFacingTarget = AIController->IsFacingTarget();

	AIController->GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), bIsFacingTarget);
}
