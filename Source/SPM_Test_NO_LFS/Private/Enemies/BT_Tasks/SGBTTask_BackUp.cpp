// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BT_Tasks/SGBTTask_BackUp.h"

#include "SPM_Test_NO_LFS.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemies/AI/SGAIControllerEnemyFlying_2.h"
#include "Enemies/Characters/SGEnemyCharacter.h"

USGBTTask_BackUp::USGBTTask_BackUp()
{
	NodeName = TEXT("Back Up");
}

EBTNodeResult::Type USGBTTask_BackUp::ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory)
{
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

	UBlackboardComponent* BTComp = Comp.GetBlackboardComponent();

	if (!BTComp)
	{
		BASIR_LOG(Error, TEXT("Back Up Task, BTComp is null"));
		return EBTNodeResult::Failed;
	}

	AActor* AttackTarget = Cast<AActor>(BTComp->GetValueAsObject("AttackTarget"));

	if (!AttackTarget)
	{
		BASIR_LOG(Error, TEXT("Back Up Task, AttackTarget is null"));
		return EBTNodeResult::Failed;
	}

	if (const FVector RetreatTargetLocation = AIController->BackUpBeforeCharging(); FVector::DistSquared(ControlledEnemy->GetActorLocation(), RetreatTargetLocation) < 20)
	{
		AIController->GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), true);
		BASIR_LOG(Error, TEXT("Back Up Task"));
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}

