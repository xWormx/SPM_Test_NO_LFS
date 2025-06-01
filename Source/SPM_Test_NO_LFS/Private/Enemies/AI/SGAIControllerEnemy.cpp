#include "Enemies/AI/SGAIControllerEnemy.h"

#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "SPM_Test_NO_LFS.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemies/Characters/SGEnemyCharacter.h"
#include "Enemies/Navigation/SGEnemyPatrolPoint.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"


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

	if (BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);
		AttackTarget = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		GetBlackboardComponent()->SetValueAsObject(TEXT("AttackTarget"), AttackTarget);
		
	}
	UpdatePatrolPoints();
}

void ASGAIControllerEnemy::SetShouldPatrol()
{
	bShouldPatrol = true;
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
	
	ControlledEnemy->GetCharacterMovement()->GravityScale = 1.f;
	
	Super::Tick(DeltaTime);
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

	if (bIsStuck)
	{
		UE_LOG(LogTemp, Warning, TEXT("Stuck"));
	}

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


