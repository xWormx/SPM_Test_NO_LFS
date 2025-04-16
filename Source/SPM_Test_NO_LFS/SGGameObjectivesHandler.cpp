// Fill out your copyright notice in the Description page of Project Settings.


#include "SGGameObjectivesHandler.h"
#include "SGEnemyCharacter.h"

// Sets default values
ASGGameObjectivesHandler::ASGGameObjectivesHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASGGameObjectivesHandler::BeginPlay()
{
	Super::BeginPlay();
	CurrentObjective = GameObjectiveOrder[0];
	if (TargetCharacter != nullptr)
		TargetCharacter->OnEnemyDied.AddDynamic(this, &ASGGameObjectivesHandler::UpdateCurrentGameObjective);
	else
		UE_LOG(LogTemp, Error, TEXT("ASGGameObjectivesHandler::BeginPlay: Must assign TargetCharacter responsible for the delegate!"));
}

void ASGGameObjectivesHandler::RegisterEnemy(ASGEnemyCharacter* Enemy)
{
	if (Enemy == nullptr)
		return;

	Enemy->OnEnemyDied.AddDynamic(this, &ASGGameObjectivesHandler::UpdateCurrentGameObjective);
}

void ASGGameObjectivesHandler::UpdateCurrentGameObjective(ASGEnemyCharacter* Actor)
{
	switch (CurrentObjective)
	{
		case EObjectiveType::EOT_KillAllEnemies:
			{
				if (EnemiesKilled == NumberOfEnemiesToKill)
				{
					FString str = FString::Printf(TEXT("Objective Completed: KillAllEnemies!"));
					GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, str);
					
				}
			} break;
		case EObjectiveType::EOT_CollectAndPlace:
			{
				if (CurrentCollectedAmout == GoalCollectiblesAmount)
				{
					FString str = FString::Printf(TEXT("Objective Completed: GoalCollectiblesAmount!"));
					GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, str);
				}
			} break;
		case EObjectiveType::EOT_DefendThePod:
			{
				if (bPodDefended)
				{
					FString str = FString::Printf(TEXT("Objective Completed: DefendThePod!"));
					GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, str);
						
				}
			} break;
	}
}

void ASGGameObjectivesHandler::StartNextObjective(EObjectiveType NextObjectiveType)
{
	CurrentObjective = NextObjectiveType;
}


