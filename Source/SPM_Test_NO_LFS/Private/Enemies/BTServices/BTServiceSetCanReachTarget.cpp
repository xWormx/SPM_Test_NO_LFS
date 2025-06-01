// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BTServices/BTServiceSetCanReachTarget.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Enemies/Characters/SGEnemyCharacter.h"

UBTServiceSetCanReachTarget::UBTServiceSetCanReachTarget()
{
	NodeName = TEXT("Set Can Reach Target");
}

void UBTServiceSetCanReachTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (!AIController || !ControlledEnemy)
	{
		return;
	}

	if (AIController->CanReachTarget(AIController->GetAttackTarget()))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), true);
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), false);
	}
}
