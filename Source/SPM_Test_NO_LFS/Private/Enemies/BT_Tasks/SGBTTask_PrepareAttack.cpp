// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BT_Tasks/SGBTTask_PrepareAttack.h"

#include "SPM_Test_NO_LFS.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemies/AI/SGAIControllerEnemyFlying_2.h"
#include "Enemies/Characters/SGEnemyCharacter.h"
#include "Enemies/Components/SGEnemyAttackComponentBase.h"

USGBTTask_PrepareAttack::USGBTTask_PrepareAttack()
{
	NodeName = TEXT("Prepare Attack");
}

EBTNodeResult::Type USGBTTask_PrepareAttack::ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory)
{
	Super::ExecuteTask(Comp, NodeMemory);

	Super::ExecuteTask(Comp, NodeMemory);

	ASGAIControllerEnemyFlying_2* AIController = Cast<ASGAIControllerEnemyFlying_2>(Comp.GetAIOwner());

	if (!AIController)
	{
		BASIR_LOG(Error, TEXT("Back Up Task, AIController is null"));
		return EBTNodeResult::Failed;
	}

	ASGEnemyCharacter* ControlledEnemy = Cast<ASGEnemyCharacter>(AIController->GetPawn());

	if (!ControlledEnemy)
	{
		BASIR_LOG(Error, TEXT("Back Up Task, Controlled Enemy is null"));
		return EBTNodeResult::Failed;
	}

	if (!AIController->GetAttackTarget())
	{
		return EBTNodeResult::Failed;
	}
	
	const FVector PreRetreatLocation = AIController->PrepareAttack();
	AIController->GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), true);

	if (FVector::DistSquared(ControlledEnemy->GetActorLocation(), PreRetreatLocation) < 200)
	{
		BASIR_LOG(Warning, TEXT("Moved Forward"));
		FVector RetreatLocation = AIController->BackUpBeforeCharging();

		if (FVector::DistSquared(ControlledEnemy->GetActorLocation(), RetreatLocation) < 200)
		{
			BASIR_LOG(Warning, TEXT("Moved backward"));
			AIController->GetControlledEnemy()->GetAttackComponent()->StartAttack(AIController->GetAttackTarget());
		}
		
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}
