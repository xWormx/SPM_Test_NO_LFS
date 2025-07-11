﻿#include "Enemies/AI/SGAIControllerEnemy.h"

#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "SPM_Test_NO_LFS.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_String.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Enemies/Characters/SGEnemyCharacter.h"
#include "Enemies/Managers/SGEnemySpawnManager.h"
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

	if (EnemyBehaviorTree)
	{
		RunBehaviorTree(EnemyBehaviorTree);
		GetBlackboardComponent()->SetValueAsObject(TEXT("AttackTarget"), AttackTarget);
		
	}
	UpdatePatrolPoints();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASGEnemySpawnManager::StaticClass(), SpawnManagers);
}

void ASGAIControllerEnemy::SetShouldPatrol()
{
	if (ControlledEnemy->IsHidden())
	{
		bShouldPatrol = false;
	}
	bShouldPatrol = true;
}

bool ASGAIControllerEnemy::IsFacingTarget() const
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

void ASGAIControllerEnemy::SetBehaviorTreeEnabled(bool bEnabled)
{
	if (!EnemyBehaviorTree)
	{
		return;
	}
	
	if (bEnabled)
	{
		RunBehaviorTree(EnemyBehaviorTree);
	}
	else
	{
		UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(BrainComponent);

		if (!BTComp)
		{
			BASIR_LOG(Error, TEXT("AI Controller, No BTComp"));
		}

		if (BTComp->IsRunning())
		{
			BTComp->StopTree(EBTStopMode::Safe);
			ClearBlackboardValues();
		}
	}
}

void ASGAIControllerEnemy::ClearBlackboardValues()
{
	UBlackboardComponent* BlackboardComp = Cast<UBlackboardComponent>(Blackboard);
	UBlackboardData* BlackboardData = BlackboardComp ? BlackboardComp->GetBlackboardAsset() : nullptr;

	if (!BlackboardData || !BlackboardComp)
	{
		BASIR_LOG(Warning, TEXT("Blackboard Component or Blackboard Data is null"));
		return;
	}

	const TArray<FBlackboardEntry>& Entries = BlackboardData->GetKeys();

	for (const FBlackboardEntry& Entry : Entries)
	{
		const UBlackboardKeyType* KeyType = Entry.KeyType;
		FName KeyName = Entry.EntryName;

		if (KeyType->IsA(UBlackboardKeyType_Bool::StaticClass()) ||
			KeyType->IsA(UBlackboardKeyType_Vector::StaticClass()))
		{
			BlackboardComp->ClearValue(KeyName);
		}
	}
}

FVector ASGAIControllerEnemy::GetFallbackChaseLocation() const
{
	if (!ControlledEnemy)
	{
		return FVector::ZeroVector;
	}
	UWorld* World = GetWorld();
	if (!World)
	{
		return FVector::ZeroVector;
	}

	UNavigationSystemV1* NavSys = Cast<UNavigationSystemV1>(World->GetNavigationSystem());
	if (!NavSys)
	{
		return FVector::ZeroVector;
	}

	FVector CurrentEnemyLocation = ControlledEnemy->GetActorLocation();

	float SearchRadius = 500.f;

	FNavLocation RandomLocation;

	NavSys->GetRandomReachablePointInRadius(CurrentEnemyLocation, SearchRadius, RandomLocation, nullptr);

	return RandomLocation;
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
		return false;
	}

	if (!ControlledEnemy)
	{
		BASIR_LOG(Warning, TEXT("Controlled Enemy null"));
		return false;
	}

	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());

	if (!NavSys)
	{
		BASIR_LOG(Warning, TEXT("NavSys is null"));
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
	if (!ControlledEnemy || ControlledEnemy->IsHidden())
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

bool ASGAIControllerEnemy::IsStuckOutsideNavMesh()
{
	if (!ControlledEnemy || ControlledEnemy->IsHidden())
	{
		return false;
	}

	UWorld* World = ControlledEnemy->GetWorld();
	if (!World)
	{
		return false;
	}

	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(World);
	if (!NavSys)
	{
		return false;
	}

	ControlledEnemyLocation = ControlledEnemy->GetActorLocation();

	
	const FVector TestLocation = ControlledEnemyLocation + FVector(0, 0, 50);
	const FVector SearchLocation = FVector(50, 50, 200);

	FNavLocation NearestNavLocation;
	
	bool bIsOnNavMesh = NavSys->ProjectPointToNavigation(TestLocation, NearestNavLocation, SearchLocation);

	if (!bIsOnNavMesh && IsStuck())
	{
		BASIR_LOG(Warning, TEXT("Enemy is not on NavMesh"));
		return true;
		
	}
	return false;
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


