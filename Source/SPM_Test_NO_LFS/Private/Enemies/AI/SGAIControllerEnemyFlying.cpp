#include "Enemies/AI/SGAIControllerEnemyFlying.h"

#include "Enemies/Characters/SGEnemyCharacter.h"
#include "Enemies/Components/SGEnemyChargeAttackComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

ASGAIControllerEnemyFlying::ASGAIControllerEnemyFlying()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASGAIControllerEnemyFlying::BeginPlay()
{
	Super::BeginPlay();
}

void ASGAIControllerEnemyFlying::HandleMovement()
{
	if (!AttackTarget || !ControlledEnemy)
	{
		return;
	}

	if (!bShouldAlwaysChaseTarget)
	{
		if (!LineOfSightTo(AttackTarget))
		{
			return;
		}
	}
	
	float TargetZ = AttackTarget->GetActorLocation().Z;	
	float HoverZ = TargetZ + FMath::Sin(GetWorld()->TimeSeconds * HoverSpeed) * HoverAmplitude;
	
	FVector CurrentLocation = ControlledEnemy->GetActorLocation();
	CurrentLocation.Z = FMath::FInterpTo(CurrentLocation.Z, HoverZ, GetWorld()->GetDeltaSeconds(), 2.0f);
	ControlledEnemy->SetActorLocation(CurrentLocation, true);
	
	if (CanAttackTarget())
	{
		bIsAttacking = true;
		ControlledEnemy->GetAttackComponent()->StartAttack(AttackTarget);
	}
	else
	{
		bIsAttacking = false;
	}

	if (!bIsAttacking)
	{
		FlyTowardsTarget();
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