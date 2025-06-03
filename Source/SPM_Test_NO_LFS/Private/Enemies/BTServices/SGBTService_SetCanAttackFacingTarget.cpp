// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BTServices/SGBTService_SetCanAttackFacingTarget.h"

#include "SPM_Test_NO_LFS.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemies/AI/SGAIControllerEnemy.h"

USGBTService_SetCanAttackFacingTarget::USGBTService_SetCanAttackFacingTarget()
{
	NodeName = TEXT("Set Can Attack Facing Target");
}

void USGBTService_SetCanAttackFacingTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	ASGAIControllerEnemy* AIController = Cast<ASGAIControllerEnemy>(OwnerComp.GetAIOwner());

	if (!AIController)
	{
		BASIR_LOG(Error, TEXT("Set Can Attack Facing Target, AIController is Null"));
		return;
	}

	const bool bCanAttackTarget = AIController->CanAttackTarget() && AIController->IsFacingTarget();

	AIController->GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), bCanAttackTarget);
	
}
