// Fill out your copyright notice in the Description page of Project Settings.


#include "SGFlyingEnemyAIController.h"

#include "Kismet/GameplayStatics.h"

void ASGFlyingEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (AIBehaviorTree)
	{
		RunBehaviorTree(AIBehaviorTree);

		//GetBlackboardComponent()
	}
}

void ASGFlyingEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetFocus(PlayerPawn);

	//MoveToActor(PlayerPawn, 200);
}
