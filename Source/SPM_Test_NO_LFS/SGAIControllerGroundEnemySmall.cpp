// Fill out your copyright notice in the Description page of Project Settings.


#include "SGAIControllerGroundEnemySmall.h"

#include "NavigationSystem.h"
#include "AI/NavigationSystemBase.h"
#include "Kismet/GameplayStatics.h"

class UNavigationSystemV1;

void ASGAIControllerGroundEnemySmall::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

void ASGAIControllerGroundEnemySmall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!PlayerPawn)
	{
		return;
	}

	if (CanShootPlayer())
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Green, TEXT("Enemy is Shooting"));
		}
	}
	
	HandleMovement();
	
}

bool ASGAIControllerGroundEnemySmall::CanShootPlayer() const
{
	FVector Location = GetPawn()->GetActorLocation();
	FVector PlayerLocation = PlayerPawn->GetActorLocation();

	float DistanceToPlayer = FVector::Dist(PlayerLocation, Location);

	bool bCanHitPlayer = DistanceToPlayer <= AttackRange - 1.f;
	return bCanHitPlayer;
}

void ASGAIControllerGroundEnemySmall::HandleMovement()
{
	FVector Location = GetPawn()->GetActorLocation();
	FVector PlayerLocation = PlayerPawn->GetActorLocation();

	float DistanceToPlayer = FVector::Dist(PlayerLocation, Location);
	

	if (DistanceToPlayer < AcceptanceRadius)
	{
		FVector DirectionAwayFromPlayer = (Location - PlayerLocation).GetSafeNormal(); // Vector pointing away from the player
		FVector MoveAwayLocation = Location + DirectionAwayFromPlayer * RetreatDistance; // Calculate a new location to retreat to
		
		MoveToLocation(MoveAwayLocation);
	}

	else
	{
		SetFocus(PlayerPawn);
		MoveToActor(PlayerPawn, AcceptanceRadius);
	}
	
}
