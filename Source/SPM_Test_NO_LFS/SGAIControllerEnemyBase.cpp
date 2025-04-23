// Fill out your copyright notice in the Description page of Project Settings.


#include "SGAIControllerEnemyBase.h"

ASGAIControllerEnemyBase::ASGAIControllerEnemyBase()
{
	AttackRange = 500.0f;
	AttackTarget = nullptr;
	AcceptanceRadius = 500.0f;
	RetreatDistance = 500.0f;
}

void ASGAIControllerEnemyBase::BeginPlay()
{
	Super::BeginPlay();
}

bool ASGAIControllerEnemyBase::CanAttackTarget() const
{
	FVector Location = GetPawn()->GetActorLocation();
	FVector TargetLocation = AttackTarget->GetActorLocation();

	float DistanceToPlayer = FVector::Dist(TargetLocation, Location);

	bool bCanAttackTarget = DistanceToPlayer < AttackRange;

	return bCanAttackTarget;
}

void ASGAIControllerEnemyBase::HandleMovement()
{
}

void ASGAIControllerEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
