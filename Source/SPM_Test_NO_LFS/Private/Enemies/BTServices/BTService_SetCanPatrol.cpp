// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BTServices/BTService_SetCanPatrol.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Enemies/AI/SGAIControllerEnemy.h"

UBTService_SetCanPatrol::UBTService_SetCanPatrol()
{
	NodeName = TEXT("Set Can Patrol");
}

void UBTService_SetCanPatrol::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ASGAIControllerEnemy* AICOntroller = Cast<ASGAIControllerEnemy>(OwnerComp.GetAIOwner());

	if (!AICOntroller)
	{
		return;
	}

	AICOntroller->PatrolDelay();
	const bool bShouldPatrol = AICOntroller->bShouldPatrol;

	AICOntroller->GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), bShouldPatrol);
}
