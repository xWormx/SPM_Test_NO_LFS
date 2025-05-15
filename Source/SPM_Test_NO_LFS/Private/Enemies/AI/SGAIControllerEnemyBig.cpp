// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/AI/SGAIControllerEnemyBig.h"

#include "Enemies/Characters/SGEnemyCharacter.h"
#include "Enemies/Components/SGEnemyMeleAttackComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ASGAIControllerEnemyBig::ASGAIControllerEnemyBig()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASGAIControllerEnemyBig::BeginPlay()
{
	Super::BeginPlay();
}

void ASGAIControllerEnemyBig::HandleMovement()
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

	if (CanReachTarget())
	{
		if (CanAttackTarget())
		{
			ControlledEnemy->GetAttackComponent()->StartAttack(AttackTarget);
		}
		else
		{
			MoveToActor(AttackTarget);
		}
	}
	else
	{
		// TODO: Patroling
		MoveToLocation(FirstStartLocation);
	}
}

void ASGAIControllerEnemyBig::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	HandleMovement();
	RotateTowardsTargetWhileNotMoving();
}

/*void ASGAIControllerEnemyBig::JumpToLocation(const FVector& Destination)
{
	if (!ControlledEnemy) return;

	FVector CurrentLocation = ControlledEnemy->GetActorLocation();
	FVector ToTarget = Destination - CurrentLocation;

	// Add vertical lift manually
	FVector JumpVelocity = ToTarget.GetSafeNormal() * 600.f; // Forward speed
	JumpVelocity.Z = 420.f; // Vertical lift — tweak as needed

	// Optional: face the jump direction
	FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(CurrentLocation, Destination);
	ControlledEnemy->SetActorRotation(NewRotation);

	ControlledEnemy->LaunchCharacter(JumpVelocity, true, true);
}*/
