// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/AI/SGAIControllerEnemySmall.h"

#include "SPM_Test_NO_LFS.h"
#include "Components/CapsuleComponent.h"
#include "Enemies/AI/SGAIControllerEnemyBig.h"
#include "Enemies/Characters/SGEnemyCharacter.h"
#include "Enemies/Components/SGEnemyShootAttackComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ASGAIControllerEnemySmall::ASGAIControllerEnemySmall()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.5f;
}

void ASGAIControllerEnemySmall::BeginPlay()
{
	Super::BeginPlay();
}

void ASGAIControllerEnemySmall::HandleMovement()
{
	FVector Location = ControlledEnemy->GetActorLocation();
	FVector PlayerLocation = AttackTarget->GetActorLocation();

	float DistanceToPlayer = FVector::Dist(PlayerLocation, Location);
	
	
	if (DistanceToPlayer < AcceptanceRadius)
	{
		FVector DirectionAwayFromPlayer = (Location - PlayerLocation).GetSafeNormal();
		FVector MoveAwayLocation = Location + DirectionAwayFromPlayer * RetreatDistance; 
		
		MoveToLocation(MoveAwayLocation);
	}

	else
	{
		MoveToActor(AttackTarget, AcceptanceRadius);
	}
}


void ASGAIControllerEnemySmall::Tick(float DeltaTime)
{
	if (!ControlledEnemy)
	{
		BASIR_LOG(Error ,TEXT("Should Retreat Service, Controlled Enemy is Null"));
		return;
	}

	if (!ControlledEnemy->IsActorTickEnabled())
	{
		ControlledEnemy->GetCapsuleComponent()->SetEnableGravity(false);
		ControlledEnemy->GetCharacterMovement()->GravityScale = 0.f;
		ControlledEnemy->GetCharacterMovement()->StopMovementImmediately();
		return;
	}

	if (ControlledEnemy->IsActorTickEnabled())
	{
		ControlledEnemy->GetCharacterMovement()->GravityScale = 1.f;
	}
	
	Super::Tick(DeltaTime);

	if (!bShouldAlwaysChaseTarget)
	{
		if (!LineOfSightTo(AttackTarget))
		{
			return;
		}
	}
	
	RotateTowardsTargetWhileNotMoving();
	
	if (CanAttackTarget())
	{
		ControlledEnemy->GetAttackComponent()->StartAttack(AttackTarget);
	}

	if (CanReachTarget(AttackTarget))
	{
		GetWorld()->GetTimerManager().ClearTimer(PatrolDelayTimerHandle);
		bShouldPatrol = false;
		HandleMovement();
	}
	else
	{
		if (!GetWorld()->GetTimerManager().IsTimerActive(PatrolDelayTimerHandle))
		{
			GetWorld()->GetTimerManager().SetTimer(
				PatrolDelayTimerHandle,
				this,
				&ASGAIControllerEnemySmall::PatrolDelay,
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

bool ASGAIControllerEnemySmall::CanAttackTarget() const
{
	return Super::CanAttackTarget() & IsFacingTarget();
}