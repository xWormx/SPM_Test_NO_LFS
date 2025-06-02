// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BTServices/SGBTService_SetTargetLocation.h"

#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemies/AI/SGAIControllerEnemy.h"
#include "Enemies/Characters/SGEnemyCharacter.h"

USGBTService_SetTargetLocation::USGBTService_SetTargetLocation()
{
	NodeName = TEXT("Set Target Location");
}

void USGBTService_SetTargetLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ASGAIControllerEnemy* AIController = Cast<ASGAIControllerEnemy>(OwnerComp.GetAIOwner());

	if (!AIController)
	{
		return;
	}

	ASGEnemyCharacter* ControlledEnemy = Cast<ASGEnemyCharacter>(AIController->GetPawn());
	AActor* AttackTarget = Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject("AttackTarget"));

	if (!ControlledEnemy || !AttackTarget)
	{
		return;
	}

	FVector TargetLocation = AttackTarget->GetActorLocation();
	
	FVector DownwardPoint = TargetLocation - FVector(0, 0, 1000);

	
	FVector SearchExtent(100.0f, 100.0f, 500.0f);

	FNavLocation ProjectedLocation;

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	
	if (NavSys && NavSys->ProjectPointToNavigation(DownwardPoint, ProjectedLocation, SearchExtent))
	{
		AIController->GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), ProjectedLocation);
	}
	else
	{
		FVector NewLocation = FVector(
			AttackTarget->GetActorLocation().X,
			AttackTarget->GetActorLocation().Y,
			ControlledEnemy->GetActorLocation().Z - 10.f
		);

		AIController->GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), NewLocation);
	}
}
