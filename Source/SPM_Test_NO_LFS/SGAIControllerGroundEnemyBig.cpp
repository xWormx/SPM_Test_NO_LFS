// Fill out your copyright notice in the Description page of Project Settings.


#include "SGAIControllerGroundEnemyBig.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void ASGAIControllerGroundEnemyBig::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (AIBehaviorTree)
	{
		RunBehaviorTree(AIBehaviorTree);

		GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"),GetPawn()->GetActorLocation());
	}
}

void ASGAIControllerGroundEnemyBig::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}




