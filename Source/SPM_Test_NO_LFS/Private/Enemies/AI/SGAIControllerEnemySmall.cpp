// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/AI/SGAIControllerEnemySmall.h"

#include "Enemies/Characters/SGEnemyCharacter.h"
#include "Enemies/Components/SGEnemyShootAttackComponent.h"

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

	if (!bShouldAlwaysChaseTarget)
	{
		if (!LineOfSightTo(AttackTarget))
		{
			return;
		}
	}
	FVector Location = GetPawn()->GetActorLocation();
	FVector PlayerLocation = AttackTarget->GetActorLocation();

	float DistanceToPlayer = FVector::Dist(PlayerLocation, Location);
	

	if (DistanceToPlayer < AcceptanceRadius)
	{
		FVector DirectionAwayFromPlayer = (Location - PlayerLocation).GetSafeNormal();
		FVector MoveAwayLocation = Location + DirectionAwayFromPlayer * RetreatDistance; 
		
		MoveToLocation(MoveAwayLocation);
	}

	else
	{
		MoveToActor(AttackTarget, AcceptanceRadius);
	}

	ControlledCharacter = Cast<ASGEnemyCharacter>(GetPawn());

	if (CanAttackTarget())
	{
		if (ControlledCharacter)
		{
			if (USGEnemyShootAttackComponent* ShootAttackComponent = ControlledCharacter->FindComponentByClass<USGEnemyShootAttackComponent>())
			{
				ShootAttackComponent->StartAttack(AttackTarget);
			}
		}
	}
}

void ASGAIControllerEnemySmall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	
	HandleMovement();
	
}
