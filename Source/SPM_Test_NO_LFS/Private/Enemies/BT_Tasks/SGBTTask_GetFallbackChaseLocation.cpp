// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BT_Tasks/SGBTTask_GetFallbackChaseLocation.h"

#include "SPM_Test_NO_LFS.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemies/AI/SGAIControllerEnemyFlying_2.h"

USGBTTask_GetFallbackChaseLocation::USGBTTask_GetFallbackChaseLocation()
{
	NodeName = TEXT("Get Fallback Chase Location");
}

EBTNodeResult::Type USGBTTask_GetFallbackChaseLocation::ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory)
{
	Super::ExecuteTask(Comp, NodeMemory);

	ASGAIControllerEnemyFlying_2* AIController = Cast<ASGAIControllerEnemyFlying_2>(Comp.GetAIOwner());

	if (!AIController)
	{
		BASIR_LOG(Error, TEXT("Set Fallback Chase Location, AIController is Null"));
		return EBTNodeResult::Failed;
	}

	const FVector FallbackChaseLocation = AIController->GetFallbackChaseLocation();

	if (FallbackChaseLocation == FVector::ZeroVector)
	{
		return EBTNodeResult::Succeeded;
	}

	AIController->GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), FallbackChaseLocation);
	return EBTNodeResult::Succeeded;
}
