
#include "Enemies/AI/SGAIControllerEnemyFlying_2.h"

#include "SPM_Test_NO_LFS.h"
#include "Enemies/Characters/SGEnemyCharacter.h"
#include "Enemies/Navigation/SGEnemyMoveToPoint.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"


ASGAIControllerEnemyFlying_2::ASGAIControllerEnemyFlying_2()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called every frame
void ASGAIControllerEnemyFlying_2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetFlyingMode(true);
	//PrepareAttack();
}

void ASGAIControllerEnemyFlying_2::SetFlyingMode(bool bShouldFly)
{
	/*if (!ControlledEnemy)
	{
		return;
	}
	
	ControlledEnemyLocation = ControlledEnemy->GetActorLocation();

	const float ZValue = ControlledEnemyLocation.Z;
	
	const float HoverZ = ZValue + FMath::Sin(GetWorld()->TimeSeconds * HoverSpeed) * HoverAmplitude;
	
	
	ControlledEnemyLocation.Z = FMath::FInterpTo(ZValue, HoverZ, GetWorld()->GetDeltaSeconds(), HoverInterpSpeed);
	
	ControlledEnemy->SetActorLocation(ControlledEnemyLocation, true);*/

	if (!ControlledEnemy)
	{
		return;
	}

	FVector OriginalLocation = ControlledEnemy->GetActorLocation();
	ControlledEnemyLocation = ControlledEnemy->GetActorLocation();

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
	
	FVector NewLocation = FMath::VInterpTo(ControlledEnemyLocation, TargetOrbitLocation, GetWorld()->GetDeltaSeconds(), HoverInterpSpeed);

	ControlledEnemy->SetActorLocation(NewLocation, true);
}

FVector ASGAIControllerEnemyFlying_2::GetFallbackChaseLocation() const
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
	
	RandomDirection = FMath::VRand();
	RandomDirection.Z = 0.0f;
	RandomDirection.Normalize();

	DesiredLocation = EnemyLocation + RandomDirection * HorizontalOffset;
	DesiredLocation.Z += FMath::RandRange(-VerticalOffset, VerticalOffset);

	return DesiredLocation;
}

void ASGAIControllerEnemyFlying_2::FlyTowardsLocation(const FVector& TargetLocation)
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

void ASGAIControllerEnemyFlying_2::Patrol()
{
	if (MoveToPoints.Num() <= 1)
	{
		UpdateMoveToPoints();
	}

	if (!CurrentMoveToPoint || HasReachedCurrentMoveToPoint(200.f))
	{
		CurrentMoveToPoint = GetRandomMoveToPoint();
	}

	if (CurrentMoveToPoint)
	{
		FlyTowardsLocation(CurrentMoveToPoint->GetActorLocation());
	}
}

FVector ASGAIControllerEnemyFlying_2::PrepareAttack()
{
	if (!ControlledEnemy || !AttackTarget)
	{
		return FVector::ZeroVector;
	}
	FVector Location = ControlledEnemy->GetActorLocation();
	FVector TargetLocation = AttackTarget->GetActorLocation();

	FVector Direction = (TargetLocation - Location).GetSafeNormal();
	FVector PreRetreatLocation = Location + Direction * BackUpDistance;
	FVector NewLocation = FMath::VInterpTo(Location, PreRetreatLocation, GetWorld()->GetDeltaSeconds(), BackUpSpeed);
	
	FRotator LookRotation = Direction.ToOrientationRotator();
	FRotator CurrentRotation = ControlledEnemy->GetActorRotation();
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, LookRotation, GetWorld()->GetDeltaSeconds(), BackUpSpeed);

	
	ControlledEnemy->SetActorRotation(LookRotation);
	ControlledEnemy->SetActorLocation(NewLocation, true);

	return PreRetreatLocation;
}

FVector& ASGAIControllerEnemyFlying_2::BackUpBeforeCharging()
{
	if (!ControlledEnemy || !AttackTarget)
	{
		return  RetreatTargetLocation;
	}
	FVector Location = ControlledEnemy->GetActorLocation();
	FVector TargetLocation = AttackTarget->GetActorLocation();

	FVector Direction = (Location - TargetLocation).GetSafeNormal();

	RetreatTargetLocation = Location + Direction * BackUpDistance;

	FVector NewLocation = FMath::VInterpTo(Location, RetreatTargetLocation, GetWorld()->GetDeltaSeconds(), BackUpSpeed);

	ControlledEnemy->SetActorLocation(NewLocation, true);

	return RetreatTargetLocation;
}


void ASGAIControllerEnemyFlying_2::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASGEnemyMoveToPoint::StaticClass(), MoveToPoints);

	UpdateMoveToPoints();

	bShouldBeFlying = true;
	
}

void ASGAIControllerEnemyFlying_2::UpdateMoveToPoints()
{
	MoveToPoints.Empty();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASGEnemyMoveToPoint::StaticClass(), MoveToPoints);
	
	MoveToPoints.RemoveAll( [this] (AActor* MoveToPoint)
	{
		return !LineOfSightTo(MoveToPoint);
	});
}

bool ASGAIControllerEnemyFlying_2::HasReachedCurrentMoveToPoint(float Tolerance) const
{
	if (!ControlledEnemy || !CurrentMoveToPoint)
	{
		return false;
	}
	return FVector::DistSquared(CurrentMoveToPoint->GetActorLocation(), ControlledEnemy->GetActorLocation()) <
		FMath::Square(Tolerance);
}

class ASGEnemyMoveToPoint* ASGAIControllerEnemyFlying_2::GetRandomMoveToPoint() const
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



