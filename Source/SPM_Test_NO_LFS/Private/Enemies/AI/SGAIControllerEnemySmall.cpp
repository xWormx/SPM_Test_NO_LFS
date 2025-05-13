// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/AI/SGAIControllerEnemySmall.h"

#include "Enemies/Characters/SGEnemyCharacter.h"
#include "Enemies/Components/SGEnemyShootAttackComponent.h"
#include "GameFramework/PawnMovementComponent.h"

ASGAIControllerEnemySmall::ASGAIControllerEnemySmall()
{
	PrimaryActorTick.bCanEverTick = true;
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

	if (CanAttackTarget() && IsFacingTarget())
	{
		ControlledEnemy->GetAttackComponent()->StartAttack(AttackTarget);
	}
}


void ASGAIControllerEnemySmall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bShouldAlwaysChaseTarget)
	{
		if (!LineOfSightTo(AttackTarget))
		{
			return;
		}
	}

	if (CanReachTarget())
	{
		HandleMovement();
	}
	RotateTowardsTargetWhileNotMoving();
	
}
