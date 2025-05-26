// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/AI/SGAIControllerEnemyBase.h"

#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "SPM_Test_NO_LFS.h"
#include "Enemies/Characters/SGEnemyCharacter.h"
#include "Enemies/Navigation/SGEnemyPatrolPoint.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ASGAIControllerEnemyBase::ASGAIControllerEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASGAIControllerEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	/*AttackTarget = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	ControlledEnemy = Cast<ASGEnemyCharacter>(GetPawn());*/

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

	/*if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Distance to player: %f"), DistanceToPlayer));
	}*/

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
	float ToleranceDegree = 70.f;

	FVector ControlledEnemyDirectionToTarget = (AttackTarget->GetActorLocation() - ControlledEnemy->GetActorLocation()).
		GetSafeNormal();
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

	float VelocityThreshhold = 150.0f;

	if (ControlledEnemy->GetVelocity().Size() > VelocityThreshhold)
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

	// Updating for next checking
	LastLocationCheck = CurrentLocation;
	LastLocationCheckTime = CurrentTime;
	bWasStuckLastChecked = bIsStuck;

	return bIsStuck;
}

void ASGAIControllerEnemyBase::SetInitialValues()
{
	AttackTarget = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	ControlledEnemy = Cast<ASGEnemyCharacter>(GetPawn());

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASGEnemyPatrolPoint::StaticClass(), PatrolPoints);

	UpdatePatrolPoints();
}

void ASGAIControllerEnemyBase::UpdatePatrolPoints()
{
	for (AActor* Target : PatrolPoints)
	{
		if (!CanReachTarget(Target))
		{
			PatrolPoints.Remove(Target);
		}
	}
}

ASGEnemyPatrolPoint* ASGAIControllerEnemyBase::GetPatrolPoint()
{
	if (PatrolPoints.IsEmpty())
	{
		return nullptr;
	}
	INT32 index = FMath::RandRange(0, PatrolPoints.Num() - 1);

	if (!PatrolPoints[index]) { return nullptr; }

	CurrentPatrolPoint = Cast<ASGEnemyPatrolPoint>(PatrolPoints[index]);
	
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
		/*if (!CanReachTarget(CurrentPatrolPoint))
		{
			//BASIR_DEBUG(TEXT("Move to Patrol Point"), FColor::Blue, 3.f);
			UpdatePatrolPoints();
		}*/
	}
	
}

void ASGAIControllerEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*if (!AttackTarget)
	{
		AttackTarget = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	}

	if (!ControlledEnemy)
	{
		ControlledEnemy = Cast<ASGEnemyCharacter>(GetPawn());
	}

	if (ControlledEnemy && !bIsFirstStartLocationSet)
	{
		FirstStartLocation = ControlledEnemy->GetActorLocation();
		bIsFirstStartLocationSet = true;
	}*/
}
