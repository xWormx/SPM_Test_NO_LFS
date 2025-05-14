#include "Enemies/AI/SGAIControllerEnemyFlying.h"

#include "Enemies/Characters/SGEnemyCharacter.h"
#include "Enemies/Components/SGEnemyChargeAttackComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "Enemies/Components/SGEnemyMoveToPoint.h"
#include "Kismet/GameplayStatics.h"

ASGAIControllerEnemyFlying::ASGAIControllerEnemyFlying()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASGAIControllerEnemyFlying::BeginPlay()
{
	Super::BeginPlay();

	float RandomOffset = FMath::FRandRange(-0.4f, 0.4f);

	HoverSpeed += RandomOffset;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASGEnemyMoveToPoint::StaticClass(), MoveToPoints);
}

void ASGAIControllerEnemyFlying::SetFlyingMode(bool bShouldFly)
{
	if (!bShouldFly)
	{
		return;
	}
	
	const float TargetZ = AttackTarget->GetActorLocation().Z;
	const float HoverZ = TargetZ + FMath::Sin(GetWorld()->TimeSeconds * HoverSpeed) * HoverAmplitude;

	FVector CurrentLocation = ControlledEnemy->GetActorLocation();
	CurrentLocation.Z = FMath::FInterpTo(CurrentLocation.Z, HoverZ, GetWorld()->GetDeltaSeconds(), HoverInterpSpeed);
	ControlledEnemy->SetActorLocation(CurrentLocation, true);
}

void ASGAIControllerEnemyFlying::HandleMovement()
{
}

void ASGAIControllerEnemyFlying::FlyTowardsTarget()
{
	if (!AttackTarget)
	{
		return;
	}

	FVector ToPlayer = AttackTarget->GetActorLocation() - ControlledEnemy->GetActorLocation();

	FVector Direction = ToPlayer.GetSafeNormal();

	FVector NewVelocity = Direction * ControlledEnemy->GetCharacterMovement()->MaxFlySpeed;

	ControlledEnemy->GetCharacterMovement()->Velocity = NewVelocity;

	FRotator LookRotation = FRotationMatrix::MakeFromX(Direction).Rotator();

	ControlledEnemy->SetActorRotation(LookRotation);
}

void ASGAIControllerEnemyFlying::FlyTowardsLocation(const FVector& TargetLocation)
{
	if (!ControlledEnemy)
	{
		return;
	}

	FVector ToTarget = TargetLocation - ControlledEnemy->GetActorLocation();
	FVector Direction = ToTarget.GetSafeNormal();

	FVector NewVelocity = Direction * ControlledEnemy->GetCharacterMovement()->MaxFlySpeed;
	ControlledEnemy->GetCharacterMovement()->Velocity = NewVelocity;

	FRotator LookRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
	ControlledEnemy->SetActorRotation(LookRotation);
}

ASGEnemyMoveToPoint* ASGAIControllerEnemyFlying::GetClosestMoveToPoint()
{
	ASGEnemyMoveToPoint* Closest = nullptr;

	float ClosestDistanceSquared = TNumericLimits<float>::Max();
	const FVector Location = ControlledEnemy->GetActorLocation();

	for (AActor* Actor : MoveToPoints)
	{
		if (ASGEnemyMoveToPoint* MoveToPoint = Cast<ASGEnemyMoveToPoint>(Actor))
		{
			if (!MoveToPoint || MoveToPoint == CurrentMoveToPoint)
			{
				continue;
			}

			float DistSq = FVector::DistSquared(MoveToPoint->GetActorLocation(), Location);
			if (DistSq < ClosestDistanceSquared)
			{
				Closest = MoveToPoint;
				ClosestDistanceSquared = DistSq;
			}
		}
	}

	CurrentMoveToPoint = Closest;

	if (CurrentMoveToPoint)
	{
		return CurrentMoveToPoint;
	}
	return nullptr;
}

FVector ASGAIControllerEnemyFlying::GetClosestMoveToPointLocation()
{
	ASGEnemyMoveToPoint* Closest = nullptr;

	float ClosestDistanceSquared = TNumericLimits<float>::Max();
	const FVector Location = ControlledEnemy->GetActorLocation();

	for (AActor* Actor : MoveToPoints)
	{
		if (ASGEnemyMoveToPoint* MoveToPoint = Cast<ASGEnemyMoveToPoint>(Actor))
		{
			if (!MoveToPoint || MoveToPoint == CurrentMoveToPoint)
			{
				continue;
			}

			float DistSq = FVector::DistSquared(MoveToPoint->GetActorLocation(), Location);
			if (DistSq < ClosestDistanceSquared)
			{
				Closest = MoveToPoint;
				ClosestDistanceSquared = DistSq;
			}
		}
	}

	CurrentMoveToPoint = Closest;

	if (CurrentMoveToPoint)
	{
		return CurrentMoveToPoint->GetActorLocation();
	}
	return FVector::ZeroVector;
}

bool ASGAIControllerEnemyFlying::HasReachedCurrentMoveToPoint(float Tolerance) const
{
	if (!ControlledEnemy || !CurrentMoveToPoint)
	{
		return false;
	}
	return FVector::DistSquared(CurrentMoveToPoint->GetActorLocation(), ControlledEnemy->GetActorLocation()) <
		FMath::Square(Tolerance);
}

void ASGAIControllerEnemyFlying::SetChaseAndAttackMode()
{
	if (IsStuck() && !LineOfSightTo(AttackTarget))
	{
		bHasFoundTarget = false;
	}
	else
	{
		if (CanAttackTarget())
		{
			if (IsStuck())
			{
				FVector MoveToLocation = GetFallbackChaseLocation();
				FlyTowardsLocation(MoveToLocation);
			}
			else
			{
				ControlledEnemy->GetAttackComponent()->StartAttack(AttackTarget);
				//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Cyan, TEXT("Is Charging"));
			}
		}
		else
		{
			FlyTowardsTarget();
		}
	}
}

void ASGAIControllerEnemyFlying::SearchForTarget()
{
	if (LineOfSightTo(AttackTarget))
	{
		bHasFoundTarget = true;
	}
	if (!CurrentMoveToPoint || HasReachedCurrentMoveToPoint(400.f))
	{
		CurrentMoveToPoint = GetClosestMoveToPoint();
	}

	if (CurrentMoveToPoint)
	{
		FlyTowardsLocation(CurrentMoveToPoint->GetActorLocation());
	}
}

FVector ASGAIControllerEnemyFlying::GetFallbackChaseLocation() const
{
	if (!ControlledEnemy || !AttackTarget)
	{
		return ControlledEnemy ? ControlledEnemy->GetActorLocation() : FVector::ZeroVector;
	}

	const FVector EnemyLocation = ControlledEnemy->GetActorLocation();
	const FVector TargetLocation = AttackTarget->GetActorLocation();

	
	FVector Direction = (TargetLocation - EnemyLocation).GetSafeNormal();
	
	const float HorizontalOffset = 500.0f;
	const float VerticalOffset = 300.0f;
	const float SearchRadius = 100.0f;

	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavSys)
	{
		return EnemyLocation;
	}
	
	FVector DesiredLocation = EnemyLocation + Direction * HorizontalOffset;
	DesiredLocation.Z += FMath::RandRange(-VerticalOffset, VerticalOffset);

	FNavLocation ProjectedLocation;
	bool bFound = NavSys->ProjectPointToNavigation(
		DesiredLocation,
		ProjectedLocation,
		FVector(SearchRadius, SearchRadius, VerticalOffset),
		nullptr
	);

	return bFound ? ProjectedLocation.Location : EnemyLocation;
}

void ASGAIControllerEnemyFlying::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*if (LineOfSightTo(AttackTarget))
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Has line of sight to target"));
		}
	}*/

	if (!AttackTarget || !ControlledEnemy)
	{
		return;
	}

	SetFlyingMode(true);

	if (bHasFoundTarget)
	{
		CurrentMoveToPoint = nullptr;

		SetChaseAndAttackMode();
	}
	else
	{
		SearchForTarget();
	}
}
