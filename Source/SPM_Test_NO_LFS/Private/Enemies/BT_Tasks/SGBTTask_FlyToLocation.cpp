// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BT_Tasks/SGBTTask_FlyToLocation.h"

#include "SPM_Test_NO_LFS.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemies/AI/SGAIControllerEnemyFlying_2.h"

USGBTTask_FlyToLocation::USGBTTask_FlyToLocation()
{
	NodeName = TEXT("Fly To Location");
}

EBTNodeResult::Type USGBTTask_FlyToLocation::ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory)
{
	Super::ExecuteTask(Comp, NodeMemory);

	ASGAIControllerEnemyFlying_2* AIController = Cast<ASGAIControllerEnemyFlying_2>(Comp.GetAIOwner());

	if (!AIController)
	{
		BASIR_LOG(Error, TEXT("Fly To Task, AIController is null"));
		return EBTNodeResult::Failed;
	}
	
	const FVector TargetLocation = AIController->GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());

	AIController->FlyTowardsLocation(TargetLocation);
	return EBTNodeResult::Succeeded;
}
