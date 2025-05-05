// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/AI/SGAIControllerEnemyBase.h"

#include "Enemies/Characters/SGEnemyCharacter.h"
#include "GameFramework/PawnMovementComponent.h"
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


bool ASGAIControllerEnemyBase::IsFacingTarget() const
{
	if (!ControlledEnemy || !AttackTarget)
	{
		return false;
	}
	float ToleranceDegree = 30.f;
	
	FVector ControlledEnemyDirectionToTarget = (AttackTarget->GetActorLocation() - ControlledEnemy->GetActorLocation()).GetSafeNormal();
	FVector AttackTargetDirectionToEnemy = -ControlledEnemyDirectionToTarget;

	FVector ForwardControlledEnemy = ControlledEnemy->GetActorForwardVector();
	FVector ForwardAttackTarget = AttackTarget->GetActorForwardVector();

	float DotControlledEnemy = FVector::DotProduct(ForwardControlledEnemy, ControlledEnemyDirectionToTarget);
	float DotAttackTarget = FVector::DotProduct(ForwardAttackTarget, AttackTargetDirectionToEnemy);

	float DotTolerance = FMath::Cos(FMath::DegreesToRadians(ToleranceDegree));
	
	return (DotControlledEnemy >= DotTolerance && DotAttackTarget >= DotTolerance);
}

void ASGAIControllerEnemyBase::RotateTowardsTargetWhileNotMoving()
{
	if (!ControlledEnemy)
	{
		return;
	}

	if (ControlledEnemy->GetMovementComponent()->Velocity != FVector::ZeroVector)
	{
		return;
	}
	
	FVector Direction = (AttackTarget->GetActorLocation() - ControlledEnemy->GetActorLocation()).GetSafeNormal();
	FRotator NewRotation = Direction.Rotation();
	NewRotation.Pitch = 0.f;
	NewRotation.Roll = 0.f;

	ControlledEnemy->SetActorRotation(NewRotation);
}

void ASGAIControllerEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!AttackRange)
	{
		AttackTarget = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	}

	if (!ControlledEnemy)
	{
		ControlledEnemy = Cast<ASGEnemyCharacter>(GetPawn());
	}
}
