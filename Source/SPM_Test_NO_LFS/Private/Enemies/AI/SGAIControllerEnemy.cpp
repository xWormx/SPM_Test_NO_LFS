#include "Enemies/AI/SGAIControllerEnemy.h"

#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "SPM_Test_NO_LFS.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemies/Characters/SGEnemyCharacter.h"
#include "Enemies/Navigation/SGEnemyPatrolPoint.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


ASGAIControllerEnemy::ASGAIControllerEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASGAIControllerEnemy::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		this,
		&ASGAIControllerEnemy::SetInitialValues,
		1.f,
		false
		);
}

void ASGAIControllerEnemy::SetInitialValues()
{
	ControlledEnemy = Cast<ASGEnemyCharacter>(GetPawn());
	AttackTarget = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	
	if (!ControlledEnemy && !AttackTarget)
	{
		return;
	}

	if (BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);
		GetBlackboardComponent()->SetValueAsObject(TEXT("AttackTarget"), AttackTarget);
		
	}
	UpdatePatrolPoints();
}

void ASGAIControllerEnemy::SetShouldPatrol()
{
	bShouldPatrol = true;
}

bool ASGAIControllerEnemy::IsFacingTarget() const
{
	if (!ControlledEnemy || !AttackTarget)
	{
		return false;
	}

	float ToleranceDegree = 10.f;

	FVector DirectionToTarget = (AttackTarget->GetActorLocation() - ControlledEnemy->GetActorLocation()).GetSafeNormal();
	FVector ForwardVector = ControlledEnemy->GetActorForwardVector();

	float DotProduct = FVector::DotProduct(ForwardVector, DirectionToTarget);
	float DotTolerance = FMath::Cos(FMath::DegreesToRadians(ToleranceDegree));

	return DotProduct >= DotTolerance;
}

void ASGAIControllerEnemy::UpdatePatrolPoints()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASGEnemyPatrolPoint::StaticClass(), PatrolPoints);
	PatrolPoints.RemoveAll( [this] (AActor* PatrolPoint)
	{
		return !CanReachTarget(PatrolPoint);
	});
}

class AActor* ASGAIControllerEnemy::GetPatrolPoint()
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

void ASGAIControllerEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!ControlledEnemy)
	{
		return;
	}

	if (ControlledEnemy->IsHidden())
	{
		ControlledEnemy->GetCharacterMovement()->StopMovementImmediately();
		ControlledEnemy->GetCharacterMovement()->GravityScale = 0.f;
		return;
	}

	if (bShouldBeFlying)
	{
		return;
	}
	
	ControlledEnemy->GetCharacterMovement()->GravityScale = 1.f;
}

void ASGAIControllerEnemy::Patrol()
{
	if (IsStuck())
	{
		UpdatePatrolPoints();
		CurrentPatrolPoint = GetPatrolPoint();
	}
	
	if (!CurrentPatrolPoint || HasReachedPatrolPoint(200.f))
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

void ASGAIControllerEnemy::PatrolDelay()
{
	if (!GetWorld()->GetTimerManager().IsTimerActive(PatrolDelayTimer))
	{
		GetWorld()->GetTimerManager().SetTimer(
			PatrolDelayTimer,
			this,
			&ASGAIControllerEnemy::SetShouldPatrol,
			3.f,
			false
			);
	}
}

void ASGAIControllerEnemy::RotateTowardsTarget()
{
	if (IsFacingTarget())
	{
		return;
	}
	FVector StartLocation = ControlledEnemy->GetActorLocation();
	FVector TargetLocation = AttackTarget->GetActorLocation();

	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation, TargetLocation);

	FRotator InterpRotation = FMath::RInterpTo(ControlledEnemy->GetActorRotation(), LookAtRotation, GetWorld()->GetDeltaSeconds(), 50.f);

	ControlledEnemy->SetActorRotation(InterpRotation);
}

float ASGAIControllerEnemy::GetCharacterVelocity() const
{
	if (!ControlledEnemy)
	{
		return 0.f;
	}

	const float CurrentVelocity =
		ControlledEnemy->GetCharacterMovement()->Velocity.Length();

	return CurrentVelocity;
}


bool ASGAIControllerEnemy::CanReachTarget(AActor* Target)
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

bool ASGAIControllerEnemy::CanReachTarget()
{
	if (!AttackTarget)
	{
		BASIR_LOG(Warning, TEXT("Target is null"));
	}

	if (!ControlledEnemy)
	{
		BASIR_LOG(Warning, TEXT("Controlled Enemy null"));
	}
	if (!AttackTarget || !ControlledEnemy)
	{
		return false;
	}

	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());

	if (!NavSys)
	{
		return false;
	}

	UNavigationPath* NavPath = NavSys->FindPathToActorSynchronously(GetWorld(),
		ControlledEnemy->GetActorLocation(), AttackTarget);

	if (!NavPath || NavPath->PathPoints.Num() == 0)
	{
		return false;
	}

	return NavPath->IsValid() && !NavPath->IsPartial();
	
}

bool ASGAIControllerEnemy::CanAttackTarget()
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

bool ASGAIControllerEnemy::IsStuck()
{
	if (!ControlledEnemy)
	{
		return false;
	}

	float CurrentTime = GetWorld()->GetTimeSeconds();
	if (CurrentTime - LastLocationCheckTime < StuckCheckInterval)
	{
		return bWasStuckLastCheck;
	}

	FVector CurrentLocation = ControlledEnemy->GetActorLocation();
	float DistanceMoved = FVector::Dist(CurrentLocation, LastLocation);

	bool bIsStuck = DistanceMoved < StuckDistanceThreshold;

	LastLocation = CurrentLocation;
	LastLocationCheckTime = CurrentTime;
	bWasStuckLastCheck = bIsStuck;

	return bIsStuck;
}

bool ASGAIControllerEnemy::HasReachedPatrolPoint(float Tolerance)
{
	if (!ControlledEnemy || !CurrentPatrolPoint)
	{
		return false;
	}

	return FVector::DistSquared(CurrentPatrolPoint->GetActorLocation(), ControlledEnemy->GetActorLocation()) <
		FMath::Square(Tolerance);
}

AActor* ASGAIControllerEnemy::GetAttackTarget()
{
	return AttackTarget;
}

ASGEnemyCharacter* ASGAIControllerEnemy::GetControlledEnemy()
{
	return ControlledEnemy;
}


