// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BTServices/SGBTService_SetIsStuckOutsideNav.h"

#include "SPM_Test_NO_LFS.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemies/AI/SGAIControllerEnemy.h"

USGBTService_SetIsStuckOutsideNav::USGBTService_SetIsStuckOutsideNav()
{
	NodeName = TEXT("Set Is Stuck Outside NavMesh");
}

void USGBTService_SetIsStuckOutsideNav::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ASGAIControllerEnemy* AIController = Cast<ASGAIControllerEnemy>(OwnerComp.GetAIOwner());

	if (!AIController)
	{
		BASIR_LOG(Error, TEXT("Set Is Stuck Outside Nav, AIController is Null"));
		return;
	}

	const bool bIsStuckOutsideNavMesh = AIController->IsStuckOutsideNavMesh();

	AIController->GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), bIsStuckOutsideNavMesh);
}

