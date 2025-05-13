#include "Enemies/AI/SGAIControllerEnemyFlying.h"

#include "Enemies/Characters/SGEnemyCharacter.h"
#include "Enemies/Components/SGEnemyChargeAttackComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "AIController.h"
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

void ASGAIControllerEnemyFlying::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!AttackTarget || !ControlledEnemy)
	{
		return;
	}

	SetFlyingMode(true);

	const bool bHasLineOfSight = LineOfSightTo(AttackTarget);

	if (bHasLineOfSight)
	{
		CurrentMoveToPoint = nullptr;

		if (CanAttackTarget())
		{
			ControlledEnemy->GetAttackComponent()->StartAttack(AttackTarget);
		}
		else
		{
			FlyTowardsTarget();
		}
	}
	else
	{
		if (!CurrentMoveToPoint || HasReachedCurrentMoveToPoint(400.f))
		{
			CurrentMoveToPoint = GetClosestMoveToPoint();
		}

		if (CurrentMoveToPoint)
		{
			FlyTowardsLocation(CurrentMoveToPoint->GetActorLocation());
		}
	}
}
