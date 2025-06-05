// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BTServices/SGBTService_SetChaseFallbackLocation.h"

#include "SPM_Test_NO_LFS.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemies/AI/SGAIControllerEnemy.h"
#include "Enemies/AI/SGAIControllerEnemyFlying_2.h"

USGBTService_SetChaseFallbackLocation::USGBTService_SetChaseFallbackLocation()
{
	NodeName = TEXT("Set Fallback Chase Location");
}

void USGBTService_SetChaseFallbackLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ASGAIControllerEnemyFlying_2* AIController = Cast<ASGAIControllerEnemyFlying_2>(OwnerComp.GetAIOwner());

	if (!AIController)
	{
		BASIR_LOG(Error, TEXT("Set Fallback Chase Location, AIController is Null"));
		return;
	}

	const FVector FallbackChaseLocation = AIController->GetFallbackChaseLocation();

	if (FallbackChaseLocation == FVector::ZeroVector)
	{
		return;
	}

	AIController->GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), FallbackChaseLocation);
}
