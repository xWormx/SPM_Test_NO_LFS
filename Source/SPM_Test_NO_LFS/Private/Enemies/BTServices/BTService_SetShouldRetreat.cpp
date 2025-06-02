// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BTServices/BTService_SetShouldRetreat.h"

#include "SPM_Test_NO_LFS.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemies/AI/SGAIControllerEnemy.h"
#include "Enemies/Characters/SGEnemyCharacter.h"

UBTService_SetShouldRetreat::UBTService_SetShouldRetreat()
{
	NodeName = TEXT("Set Should Retreat");
}

void UBTService_SetShouldRetreat::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ASGAIControllerEnemy* AIController = Cast<ASGAIControllerEnemy>(OwnerComp.GetAIOwner());

	if (!AIController)
	{
		return;
	}

	ASGEnemyCharacter* ControlledEnemy = Cast<ASGEnemyCharacter>(AIController->GetPawn());

	AActor* AttackTarget = Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject("AttackTarget"));

	if (!ControlledEnemy)
	{
		BASIR_LOG(Warning ,TEXT("Should Retreat Service, Controlled Enemy is Null"));
		return;
	}

	if (!AttackTarget)
	{
		BASIR_LOG(Warning ,TEXT("Should Retreat Service, Attack Target is Null"));
		return;
	}

	FVector Location = ControlledEnemy->GetActorLocation();
	FVector PlayerLocation = AttackTarget->GetActorLocation();

	float DistanceToPlayer = FVector::Dist(PlayerLocation, Location);

	if (DistanceToPlayer < AIController->AcceptanceRadius)
	{
		AIController->GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), true);
		return;
	}

	AIController->GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), false);
}
