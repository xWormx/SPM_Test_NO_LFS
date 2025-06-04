// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BT_Tasks/SGBTTask_FlyTo.h"

#include "SPM_Test_NO_LFS.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemies/AI/SGAIControllerEnemyFlying_2.h"

USGBTTask_FlyTo::USGBTTask_FlyTo()
{
	NodeName = TEXT("Fly To");
}

EBTNodeResult::Type USGBTTask_FlyTo::ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory)
{
	Super::ExecuteTask(Comp, NodeMemory);

	ASGAIControllerEnemyFlying_2* AIController = Cast<ASGAIControllerEnemyFlying_2>(Comp.GetAIOwner());

	if (!AIController)
	{
		BASIR_LOG(Error, TEXT("Fly To Task, AIController is null"));
		return EBTNodeResult::Failed;
	}

	AActor* AttackTarget = Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey()));

	if (!AttackTarget)
	{
		BASIR_LOG(Error, TEXT("Fly To Task, Attack Target is null"));
		return EBTNodeResult::Failed;
	}
	//const FVector TargetLocation = AIController->GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());

	AIController->FlyTowardsLocation(AttackTarget->GetActorLocation());
	return EBTNodeResult::Succeeded;
}

