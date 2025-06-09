// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BTServices/SGBTService_SetRetreatLocation.h"

#include "SPM_Test_NO_LFS.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemies/AI/SGAIControllerEnemy.h"
#include "Enemies/Characters/SGEnemyCharacter.h"

USGBTService_SetRetreatLocation::USGBTService_SetRetreatLocation()
{
	NodeName = TEXT("Set Retreat Location");
}

void USGBTService_SetRetreatLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ASGAIControllerEnemy* AIController = Cast<ASGAIControllerEnemy>(OwnerComp.GetAIOwner());

	if (!AIController)
	{
		BASIR_LOG(Error ,TEXT("Set Retreat Service, AIController is Null"));
		return;
	}

	ASGEnemyCharacter* ControlledEnemy = Cast<ASGEnemyCharacter>(AIController->GetPawn());

	if (!ControlledEnemy)
	{
		BASIR_LOG(Error ,TEXT("Set Retreat Service, Controlled Enemy is Null"));
		return;
	}

	AActor* AttackTarget = Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject("AttackTarget"));

	if (!AttackTarget)
	{
		BASIR_LOG(Error ,TEXT("Set Retreat Service, Attack Target is Null"));
		return;
	}

	FVector Location = ControlledEnemy->GetActorLocation();
	FVector PlayerLocation = AttackTarget->GetActorLocation();

	FVector DirectionAwayFromPlayer = (Location - PlayerLocation).GetSafeNormal();
	FVector MoveAwayLocation = Location + DirectionAwayFromPlayer * AIController->RetreatDistance;

	AIController->GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), MoveAwayLocation);

	
}
