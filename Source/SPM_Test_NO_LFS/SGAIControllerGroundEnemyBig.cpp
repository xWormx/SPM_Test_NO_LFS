// Fill out your copyright notice in the Description page of Project Settings.


#include "SGAIControllerGroundEnemyBig.h"

#include "Kismet/GameplayStatics.h"

void ASGAIControllerGroundEnemyBig::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	
}

void ASGAIControllerGroundEnemyBig::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!PlayerPawn)
	{
		return;
	}

	SetFocus(PlayerPawn);

	

	if (CanHitPlayer())
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Green, TEXT("Enemy is attacking"));
		}
		
	}
	else
	{
		MoveToActor(PlayerPawn);
	}
}

bool ASGAIControllerGroundEnemyBig::CanHitPlayer() const
{
	FVector Location = GetPawn()->GetActorLocation();
	FVector PlayerLocation = PlayerPawn->GetActorLocation();

	float DistanceToPlayer = FVector::Dist(PlayerLocation, Location);

	bool bCanHitPlayer = DistanceToPlayer <= AttackRange -1.f;
	return bCanHitPlayer;
}




