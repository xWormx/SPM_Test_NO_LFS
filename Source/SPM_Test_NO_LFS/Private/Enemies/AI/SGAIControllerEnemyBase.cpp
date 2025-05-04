// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/AI/SGAIControllerEnemyBase.h"

#include "Enemies/Characters/SGEnemyCharacter.h"
#include "Kismet/GameplayStatics.h"

ASGAIControllerEnemyBase::ASGAIControllerEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASGAIControllerEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	AttackTarget = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	ControlledEnemy = Cast<ASGEnemyCharacter>(GetPawn());
	
}

bool ASGAIControllerEnemyBase::CanAttackTarget() const
{
	if (!AttackTarget)
	{
		return false;
	}
	const FVector Location = ControlledEnemy->GetActorLocation();
	const FVector TargetLocation = AttackTarget->GetActorLocation();

	const float DistanceToPlayer = FVector::Dist(TargetLocation, Location);

	const bool bCanAttackTarget = DistanceToPlayer < AttackRange;

	return bCanAttackTarget;
}

void ASGAIControllerEnemyBase::HandleMovement()
{
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

void ASGAIControllerEnemyBase::SetControlledCharacter(ASGEnemyCharacter* NewControlledEnemy)
{
	ControlledEnemy = NewControlledEnemy;
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
