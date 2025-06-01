// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BT_Tasks/SGBTServiceTest.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Enemies/AI/SGAIControllerEnemy.h"
#include "Enemies/Characters/SGEnemyCharacter.h"

USGBTServiceTest::USGBTServiceTest()
{
	NodeName = TEXT("BT Service Test");
}

void USGBTServiceTest::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ASGAIControllerEnemy* AIController = Cast<ASGAIControllerEnemy>(OwnerComp.GetAIOwner());

	if (!AIController)
	{
		return;
	}

	if (!AIController->CanReachTarget(AIController->GetAttackTarget()))
	{
		AActor* AttackTarget = AIController->GetAttackTarget();
		ASGEnemyCharacter* ControlledEnemy = AIController->GetControlledEnemy();
		if (!AttackTarget || !ControlledEnemy)
		{
			return;
		}
		FVector AttackTargetLocation = FVector(AttackTarget->GetActorLocation().X, AttackTarget->GetActorLocation().Y, ControlledEnemy->GetActorLocation().Z - 20.f);
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), AttackTargetLocation);
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
	}
	
}
