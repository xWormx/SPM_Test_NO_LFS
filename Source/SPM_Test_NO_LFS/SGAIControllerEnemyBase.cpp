// Fill out your copyright notice in the Description page of Project Settings.


#include "SGAIControllerEnemyBase.h"

ASGAIControllerEnemyBase::ASGAIControllerEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;
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

float ASGAIControllerEnemyBase::GetAttackRange() const
{
	return AttackRange;
}

void ASGAIControllerEnemyBase::SetAttackRange(const float NewAttackRange)
{
	AttackRange = NewAttackRange;
}

AActor* ASGAIControllerEnemyBase::GetAttackTarget() const
{
	return AttackTarget;
}

void ASGAIControllerEnemyBase::SetAttackTarget(AActor* NewAttackTarget)
{
	AttackTarget = NewAttackTarget;
}

float ASGAIControllerEnemyBase::GetAcceptanceRadius() const
{
	return AcceptanceRadius;
}

void ASGAIControllerEnemyBase::SetAcceptanceRadius(const float NewAcceptanceRadius)
{
	AcceptanceRadius = NewAcceptanceRadius;
}

float ASGAIControllerEnemyBase::GetRetreatDistance() const
{
	return RetreatDistance;
}

void ASGAIControllerEnemyBase::SetRetreatDistance(const float NewRetreatDistance)
{
	RetreatDistance = NewRetreatDistance;
}


void ASGAIControllerEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
