// Fill out your copyright notice in the Description page of Project Settings.


#include "SGGroundEnemyAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void ASGGroundEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	

	if (AIBehaviorTree)
	{
		RunBehaviorTree(AIBehaviorTree);

		GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
	}
}

void ASGGroundEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (LineOfSightTo(PlayerPawn) && PlayerPawn )
	{
		//SetFocus(PlayerPawn);
		//MoveToActor(PlayerPawn, AcceptanceRadius);
		GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
		GetBlackboardComponent()->SetValueAsVector(TEXT("LastKnownPlayerLocation"), PlayerPawn->GetActorLocation());
	}
	else
	{
		//ClearFocus(EAIFocusPriority::Gameplay);
		//StopMovement();
		GetBlackboardComponent()->ClearValue(TEXT("PlayerLocation"));
	}
}
