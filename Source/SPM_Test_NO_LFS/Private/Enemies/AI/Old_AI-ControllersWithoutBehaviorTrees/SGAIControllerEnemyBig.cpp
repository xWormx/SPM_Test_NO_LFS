// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/AI/Old_AI-ControllersWithoutBehaviorTrees/SGAIControllerEnemyBig.h"

#include "Components/CapsuleComponent.h"
#include "Enemies/Characters/SGEnemyCharacter.h"
#include "Enemies/Components/SGEnemyMeleeAttackComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Navigation/PathFollowingComponent.h"

ASGAIControllerEnemyBig::ASGAIControllerEnemyBig()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.5f;
}

void ASGAIControllerEnemyBig::BeginPlay()
{
	Super::BeginPlay();
}

void ASGAIControllerEnemyBig::HandleMovement()
{
	if (!AttackTarget || !ControlledEnemy)
	{
		return;
	}

	if (!bShouldAlwaysChaseTarget)
	{
		if (!LineOfSightTo(AttackTarget))
		{
			return;
		}
	}

	if (CanReachTarget(AttackTarget))
	{
		GetWorld()->GetTimerManager().ClearTimer(PatrolDelayTimerHandle);
		bShouldPatrol = false;

		if (CanAttackTarget())
		{
			ControlledEnemy->GetAttackComponent()->StartAttack(AttackTarget);
		}
		else
		{
			MoveToActor(AttackTarget);
		}
	}
	else
	{
		if (!GetWorld()->GetTimerManager().IsTimerActive(PatrolDelayTimerHandle))
		{
			GetWorld()->GetTimerManager().SetTimer(
				PatrolDelayTimerHandle,
				this,
				&ASGAIControllerEnemyBig::PatrolDelay,
				3.f,
				false
			);
		}
		if (bShouldPatrol)
		{
			Patrol();
		}
		else
		{
			SetAttackTargetLocation();
			MoveToLocation(AttackTargetLocation);
		}
	}

}

void ASGAIControllerEnemyBig::Tick(float DeltaTime)
{
	if (!ControlledEnemy)
	{
		return;
	}
	Super::Tick(DeltaTime);
	HandleMovement();
	RotateTowardsTargetWhileNotMoving();
}
