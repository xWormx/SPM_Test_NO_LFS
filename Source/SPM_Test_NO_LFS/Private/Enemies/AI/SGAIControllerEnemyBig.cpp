// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/AI/SGAIControllerEnemyBig.h"

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
	if (!AttackTarget)
	{
		return;
	}

	if (!LineOfSightTo(AttackTarget))
	{
		return;
	}

	if (CanAttackTarget())
	{
		//TODO: Create attack functionality 
	}
	else
	{
		MoveToActor(AttackTarget);
	}
}

void ASGAIControllerEnemyBig::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	HandleMovement();
}
