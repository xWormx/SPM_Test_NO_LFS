// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BTServices/SGBTService_SetIsMoving.h"

#include "SPM_Test_NO_LFS.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemies/AI/SGAIControllerEnemy.h"
#include "Enemies/Characters/SGEnemyCharacter.h"
#include "GameFramework/PawnMovementComponent.h"

USGBTService_SetIsMoving::USGBTService_SetIsMoving()
{
	NodeName = TEXT("Set is Moving");
}

void USGBTService_SetIsMoving::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ASGAIControllerEnemy* AIController = Cast<ASGAIControllerEnemy>(OwnerComp.GetAIOwner());

	if (!AIController)
	{
		BASIR_LOG(Error, TEXT("Set Is Moving, AIController is Null"));
		return;
	}

	const ASGEnemyCharacter* ControlledEnemy = Cast<ASGEnemyCharacter>(AIController->GetPawn());

	if (!ControlledEnemy)
	{
		BASIR_LOG(Error, TEXT("Set Is Moving, Controlled Enemy is Null"));
		return;
	}

	const float VelocityThreshold = 50.0f;

	

	if (ControlledEnemy->GetMovementComponent()->Velocity.Size() > VelocityThreshold)
	{
		BASIR_LOG(Warning, TEXT("Velocity: %f"), ControlledEnemy->GetMovementComponent()->Velocity.Size())
		AIController->GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), true);
	}
	else
	{
		AIController->GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), false);
	}
}
