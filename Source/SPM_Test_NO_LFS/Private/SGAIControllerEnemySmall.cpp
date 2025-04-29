// Fill out your copyright notice in the Description page of Project Settings.


#include "SGAIControllerEnemySmall.h"

#include "Kismet/GameplayStatics.h"

ASGAIControllerEnemySmall::ASGAIControllerEnemySmall()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASGAIControllerEnemySmall::BeginPlay()
{
	Super::BeginPlay();
}

void ASGAIControllerEnemySmall::HandleMovement()
{
	if (!AttackTarget)
	{
		return;
	}

	if (!LineOfSightTo(AttackTarget))
	{
		return;
	}
	FVector Location = GetPawn()->GetActorLocation();
	FVector PlayerLocation = AttackTarget->GetActorLocation();

	float DistanceToPlayer = FVector::Dist(PlayerLocation, Location);
	

	if (DistanceToPlayer < AcceptanceRadius)
	{
		FVector DirectionAwayFromPlayer = (Location - PlayerLocation).GetSafeNormal(); // Vector pointing away from the player postion
		FVector MoveAwayLocation = Location + DirectionAwayFromPlayer * RetreatDistance; // Calculate a new location to back up to
		
		MoveToLocation(MoveAwayLocation);
	}

	else
	{
		MoveToActor(AttackTarget, AcceptanceRadius);
	}
}

void ASGAIControllerEnemySmall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CanAttackTarget())
	{
		//TODO: Create attack functionality
	}
	
	HandleMovement();
	
}
