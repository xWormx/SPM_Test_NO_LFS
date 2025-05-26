// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/AI/SGAIControllerEnemyBase.h"

#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Enemies/Characters/SGEnemyCharacter.h"
#include "Enemies/Navigation/SGEnemyPatrolPoint.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ASGAIControllerEnemyBase::ASGAIControllerEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;
	//PrimaryActorTick.TickInterval = 0.5f;
}

void ASGAIControllerEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		this,
		&ASGAIControllerEnemyBase::SetInitialValues,
		1.f,
		false
		);
}

bool ASGAIControllerEnemyBase::CanAttackTarget() const
{
	if (!AttackTarget || !LineOfSightTo(AttackTarget))
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

	float ToleranceDegree = 70.f;

	FVector DirectionToTarget = (AttackTarget->GetActorLocation() - ControlledEnemy->GetActorLocation()).GetSafeNormal();
	FVector ForwardVector = ControlledEnemy->GetActorForwardVector();

	float DotProduct = FVector::DotProduct(ForwardVector, DirectionToTarget);
	float DotTolerance = FMath::Cos(FMath::DegreesToRadians(ToleranceDegree));

	return DotProduct >= DotTolerance;
}

void ASGAIControllerEnemyBase::RotateTowardsTargetWhileNotMoving()
{
	if (!ControlledEnemy || ControlledEnemy->GetMovementComponent()->Velocity.Size() > 150.f)
	{
		return;
	}

	float VelocityThreshold = 150.0f;

	if (ControlledEnemy->GetVelocity().Size() > VelocityThreshold)
	{
		return;
	}

	FVector StartLocation = ControlledEnemy->GetActorLocation();
	FVector TargetLocation = AttackTarget->GetActorLocation();

	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation, TargetLocation);

	ControlledEnemy->SetActorRotation(LookAtRotation);
}

bool ASGAIControllerEnemyBase::CanReachTarget(AActor* Target) const
{
	if (!Target || !ControlledEnemy)
	{
		return false;
	}

	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());

	if (!NavSys)
	{
		return false;
	}

	UNavigationPath* NavPath = NavSys->FindPathToActorSynchronously(GetWorld(),
		ControlledEnemy->GetActorLocation(), Target);

	if (!NavPath || NavPath->PathPoints.Num() == 0)
	{
		return false;
	}

	return NavPath->IsValid() && !NavPath->IsPartial();
}

bool ASGAIControllerEnemyBase::IsStuck()
{
	if (!ControlledEnemy)
	{
		return false;
	}

	float CurrentTime = GetWorld()->GetTimeSeconds();
	if (CurrentTime - LastLocationCheckTime < StuckCheckInterval)
	{
		return bWasStuckLastChecked;
	}

	FVector CurrentLocation = ControlledEnemy->GetActorLocation();
	float DistanceMoved = FVector::Dist(CurrentLocation, LastLocationCheck);

	bool bIsStuck = DistanceMoved < StuckDistanceThreshold;

	LastLocationCheck = CurrentLocation;
	LastLocationCheckTime = CurrentTime;
	bWasStuckLastChecked = bIsStuck;

	return bIsStuck;
}

void ASGAIControllerEnemyBase::SetInitialValues()
{
	AttackTarget = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	
	ControlledEnemy = Cast<ASGEnemyCharacter>(GetPawn());

	UpdatePatrolPoints();
}

void ASGAIControllerEnemyBase::UpdatePatrolPoints()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASGEnemyPatrolPoint::StaticClass(), PatrolPoints);
	PatrolPoints.RemoveAll( [this] (AActor* PatrolPoint)
	{
		return !CanReachTarget(PatrolPoint);
	});
}

ASGEnemyPatrolPoint* ASGAIControllerEnemyBase::GetPatrolPoint()
{
	if (PatrolPoints.IsEmpty())
	{
		return nullptr;
	}
	int32 Index = FMath::RandRange(0, PatrolPoints.Num() - 1);

	if (!PatrolPoints[Index]) { return nullptr; }

	CurrentPatrolPoint = Cast<ASGEnemyPatrolPoint>(PatrolPoints[Index]);
	
	if (CurrentPatrolPoint)
	{
		return CurrentPatrolPoint;
	}
	return nullptr;
}

bool ASGAIControllerEnemyBase::HasReachedCurrentPatrolPoint(float Tolerance) const
{
	if (!ControlledEnemy || !CurrentPatrolPoint)
	{
		return false;
	}

	return FVector::DistSquared(CurrentPatrolPoint->GetActorLocation(), ControlledEnemy->GetActorLocation()) <
		FMath::Square(Tolerance);
}

void ASGAIControllerEnemyBase::Patrol()
{
	if (IsStuck())
	{
		UpdatePatrolPoints();
		CurrentPatrolPoint = GetPatrolPoint();
	}
	
	if (!CurrentPatrolPoint || HasReachedCurrentPatrolPoint(200.f))
	{
		CurrentPatrolPoint = GetPatrolPoint();
		if (!CanReachTarget(CurrentPatrolPoint))
		{
			PatrolPoints.Remove(CurrentPatrolPoint);
			UpdatePatrolPoints();
			CurrentPatrolPoint = GetPatrolPoint();
		}
	}

	if (CurrentPatrolPoint)
	{
		MoveToActor(CurrentPatrolPoint);
	}
	
}

void ASGAIControllerEnemyBase::PatrolDelay()
{
	if (!AttackTarget || CanReachTarget(AttackTarget))
	{
		return;
	}
	bShouldPatrol = true;
}

void ASGAIControllerEnemyBase::SetAttackTargetLocation()
{
	if (!AttackTarget || !ControlledEnemy)
	{
		return;
	}
	
	FVector NewAttackTargetLocation = FVector(
		AttackTarget->GetActorLocation().X,
		AttackTarget->GetActorLocation().Y,
		ControlledEnemy->GetActorLocation().Z
		);
	
	AttackTargetLocation = NewAttackTargetLocation;
}

void ASGAIControllerEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
