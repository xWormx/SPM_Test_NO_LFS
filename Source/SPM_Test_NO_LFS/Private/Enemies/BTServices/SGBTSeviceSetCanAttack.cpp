// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BTServices/SGBTSeviceSetCanAttack.h"

#include "BehaviorTree/BlackboardComponent.h"

USGBTSeviceSetCanAttack::USGBTSeviceSetCanAttack()
{
	NodeName = TEXT("Set Can Attack Target");
}

void USGBTSeviceSetCanAttack::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (!AIController || !ControlledEnemy)
	{
		UE_LOG(LogTemp, Error, TEXT("AIController is NULL"));
		return;
	}

	if (AIController->CanAttackTarget())
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), true);
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), false);
	}
}
