// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/AI/SGAIControllerEnemyBig.h"

#include "Enemies/Characters/SGEnemyCharacter.h"
#include "Enemies/Components/SGEnemyMeleAttackComponent.h"
#include "Kismet/GameplayStatics.h"

ASGAIControllerEnemyBig::ASGAIControllerEnemyBig()
{
	PrimaryActorTick.bCanEverTick = true;
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

	if (CanReachTarget())
	{
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
		// TODO: Patroling
	}
}

void ASGAIControllerEnemyBig::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	HandleMovement();
	RotateTowardsTargetWhileNotMoving();
}
