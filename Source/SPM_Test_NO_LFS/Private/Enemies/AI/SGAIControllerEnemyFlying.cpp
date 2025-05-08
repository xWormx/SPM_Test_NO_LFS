#include "Enemies/AI/SGAIControllerEnemyFlying.h"

#include "Enemies/Characters/SGEnemyCharacter.h"
#include "Enemies/Components/SGEnemyChargeAttackComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "AIController.h"

ASGAIControllerEnemyFlying::ASGAIControllerEnemyFlying()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASGAIControllerEnemyFlying::BeginPlay()
{
	Super::BeginPlay();

	float RandomOffset = FMath::FRandRange(-0.4f, 0.4f);

	HoverSpeed += RandomOffset;
}

void ASGAIControllerEnemyFlying::HandleMovement()
{
	if (!AttackTarget || !ControlledEnemy)
	{
		return;
	}
	
	const bool bHasLineOfSight = LineOfSightTo(AttackTarget);

	if (!bShouldAlwaysChaseTarget && !bHasLineOfSight)
	{
		return;
	}
	
	const float TargetZ = AttackTarget->GetActorLocation().Z;
	const float HoverZ = TargetZ + FMath::Sin(GetWorld()->TimeSeconds * HoverSpeed) * HoverAmplitude;
	
	FVector CurrentLocation = ControlledEnemy->GetActorLocation();
	CurrentLocation.Z = FMath::FInterpTo(CurrentLocation.Z, HoverZ, GetWorld()->GetDeltaSeconds(), HoverInterpSpeed);
	ControlledEnemy->SetActorLocation(CurrentLocation, true);
	
	const bool bCanAttack = CanAttackTarget() && bHasLineOfSight;
	if (bCanAttack && !bIsAttacking)
	{
		bIsAttacking = true;
		ControlledEnemy->GetAttackComponent()->StartAttack(AttackTarget);
	}
	else if (!bCanAttack && bIsAttacking)
	{
		bIsAttacking = false;
		// TODO: ControlledEnemy->GetAttackComponent()->StopAttack();
	}

	
	if (!bIsAttacking)
	{
		if (!bHasLineOfSight)
		{
			EPathFollowingRequestResult::Type MoveResult = MoveToActor(AttackTarget);
			if (MoveResult != EPathFollowingRequestResult::RequestSuccessful)
			{
				FlyTowardsTarget();
			}
			else
			{
				MoveToActor(AttackTarget);
			}
		}
		else
		{
			FlyTowardsTarget();
		}
	}
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

void ASGAIControllerEnemyFlying::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	HandleMovement();
}