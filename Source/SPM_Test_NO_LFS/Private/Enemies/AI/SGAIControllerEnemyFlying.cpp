#include "Enemies/AI/SGAIControllerEnemyFlying.h"

#include "Enemies/Characters/SGEnemyCharacter.h"
#include "Enemies/Components/SGEnemyChargeAttackComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "AIController.h"
#include "Enemies/Navigation/SGEnemyMoveToPoint.h"
#include "Kismet/GameplayStatics.h"

ASGAIControllerEnemyFlying::ASGAIControllerEnemyFlying()
{
	PrimaryActorTick.bCanEverTick = true;
	//PrimaryActorTick.TickInterval = 0.5f;
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
	/*if (!bShouldFly)
	{
		return;
	}
	
	if (!ControlledEnemy)
	{
		return;
	}
	
	CurrentLocation = ControlledEnemy->GetActorLocation();

	const float ZValue = CurrentLocation.Z;
	
	const float HoverZ = ZValue + FMath::Sin(GetWorld()->TimeSeconds * HoverSpeed) * HoverAmplitude;
	
	
	CurrentLocation.Z = FMath::FInterpTo(ZValue, HoverZ, GetWorld()->GetDeltaSeconds(), HoverInterpSpeed);
	
	ControlledEnemy->SetActorLocation(CurrentLocation, true);*/


	if (!ControlledEnemy)
	{
		return;
	}

	FVector OriginalLocation = ControlledEnemy->GetActorLocation();
	CurrentLocation = ControlledEnemy->GetActorLocation();

	const float Time = GetWorld()->GetTimeSeconds();
	const float Radius = 100.0f;
	const float OrbitSpeed = 1.f;

	float XOffset = FMath::Cos(Time * OrbitSpeed) * Radius;
	float YOffset = FMath::Sin(Time * OrbitSpeed) * Radius;
	float ZOffset = FMath::Sin(Time * HoverSpeed) * HoverAmplitude;

	FVector TargetOrbitLocation = OriginalLocation;
	TargetOrbitLocation.X += XOffset;
	TargetOrbitLocation.Y += YOffset;
	TargetOrbitLocation.Z += ZOffset;
	
	FVector NewLocation = FMath::VInterpTo(CurrentLocation, TargetOrbitLocation, GetWorld()->GetDeltaSeconds(), HoverInterpSpeed);

	ControlledEnemy->SetActorLocation(NewLocation, true);
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

	//BASIR_LOG(Warning, TEXT("Flying Towards Location : %s, from Location: %s"), *TargetLocation.ToCompactString(), *ControlledEnemy->GetActorLocation().ToCompactString());

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

ASGEnemyMoveToPoint* ASGAIControllerEnemyFlying::GetRandomMoveToPoint() const
{
	if (MoveToPoints.IsEmpty())
	{
		return nullptr;
	}

	int32 Index = FMath::RandRange(0, MoveToPoints.Num() - 1);

	ASGEnemyMoveToPoint* MoveToPoint = Cast<ASGEnemyMoveToPoint>(MoveToPoints[Index]);

	if (MoveToPoint)
	{
		return MoveToPoint;
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

	if (MoveToPoints.Num() == 1)
	{
		UpdateMoveToPoints();
	}
	if (!CurrentMoveToPoint || HasReachedCurrentMoveToPoint(400.f))
	{
		CurrentMoveToPoint = GetRandomMoveToPoint();
	}

	if (CurrentMoveToPoint)
	{
		FlyTowardsLocation(CurrentMoveToPoint->GetActorLocation());
	}
}

FVector ASGAIControllerEnemyFlying::GetFallbackChaseLocation() const
{
	if (!ControlledEnemy)
	{
		return FVector::ZeroVector;
	}

	const FVector EnemyLocation = ControlledEnemy->GetActorLocation();

	const float HorizontalOffset = 1000.0f;
	const float VerticalOffset = 600.0f;
	
	FVector RandomDirection = FMath::VRand();
	RandomDirection.Z = 0.0f;
	RandomDirection.Normalize();
	
	FVector DesiredLocation = EnemyLocation + RandomDirection * HorizontalOffset;
	DesiredLocation.Z += FMath::RandRange(-VerticalOffset, VerticalOffset);
	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(ControlledEnemy);
	
	FCollisionShape CollisionSphere = FCollisionShape::MakeSphere(50.f);

	bool bIsBlocked = GetWorld()->OverlapAnyTestByChannel(
		DesiredLocation,
		FQuat::Identity,
		ECC_WorldStatic,  
		CollisionSphere,
		QueryParams
	);

	if (!bIsBlocked)
	{
		return DesiredLocation;
	}
	
	return GetFallbackChaseLocation();
}

void ASGAIControllerEnemyFlying::UpdateMoveToPoints()
{
	MoveToPoints.Empty();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASGEnemyMoveToPoint::StaticClass(), MoveToPoints);
	
	MoveToPoints.RemoveAll( [this] (AActor* MoveToPoint)
	{
		return !LineOfSightTo(MoveToPoint);
	});
}

void ASGAIControllerEnemyFlying::Tick(float DeltaTime)
{

	if (!ControlledEnemy)
	{
		return;
	}

	if (!ControlledEnemy->IsActorTickEnabled())
	{
		return;
	}
	Super::Tick(DeltaTime);

	/*if (LineOfSightTo(AttackTarget))
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Has line of sight to target"));
		}
	}*/

	/*if (!AttackTarget || !ControlledEnemy)
	{
		return;
	}

	

	if (bHasFoundTarget)
	{
		SetFlyingMode(true);
		CurrentMoveToPoint = nullptr;

		SetChaseAndAttackMode();
	}
	else
	{
		SearchForTarget();
	}*/

	//SetFlyingMode(true);
	FVector MoveToLocation = GetFallbackChaseLocation();
	FlyTowardsLocation(MoveToLocation);
}

void ASGAIControllerEnemyFlying::HoverZDelay()
{
	const float TargetZ = AttackTarget->GetActorLocation().Z;
	float HoverZ = TargetZ + FMath::Sin(GetWorld()->TimeSeconds * HoverSpeed) * HoverAmplitude;
	CurrentLocation.Z = FMath::FInterpTo(CurrentLocation.Z, HoverZ, GetWorld()->GetDeltaSeconds(), HoverInterpSpeed);

	//CurrentLocation = ControlledEnemy->GetActorLocation();
	//ControlledEnemy->SetActorLocation(CurrentLocation, true);
	
}
