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

}

void ASGGameObjectivesHandler::RegisterEnemy(ASGEnemyCharacter* Enemy)
{
	if (Enemy == nullptr)
		return;

	Enemy->OnEnemyDied.AddDynamic(this, &ASGGameObjectivesHandler::UpdateCurrentGameObjective);
	TargetCharacters.Push(Enemy);
}

void ASGGameObjectivesHandler::UpdateCurrentGameObjective(ASGEnemyCharacter* Actor)
{
	bool CurrenObjectiveDone = false;
	switch (CurrentObjective)
	{
		case EObjectiveType::EOT_KillAllEnemies:
			{
				EnemiesKilled++;
				if (EnemiesKilled == NumberOfEnemiesToKill)
				{
					FString str = FString::Printf(TEXT("Objective Completed: KillAllEnemies!"));
					GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, str);
					CurrenObjectiveDone = true;
				}
			} break;
		case EObjectiveType::EOT_CollectAndPlace:
			{
				CurrentCollectedAmout++;
				if (CurrentCollectedAmout == GoalCollectiblesAmount)
				{
					FString str = FString::Printf(TEXT("Objective Completed: GoalCollectiblesAmount!"));
					GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, str);
					CurrenObjectiveDone = true;
				}
			} break;
		case EObjectiveType::EOT_DefendThePod:
			{
				bPodDefended = true;
				if (bPodDefended)
				{
					FString str = FString::Printf(TEXT("Objective Completed: DefendThePod!"));
					GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, str);
					CurrenObjectiveDone = true;
				}
			} break;
	}

	if (CurrenObjectiveDone && ObjectiveCounter < GameObjectiveOrder.Num() - 1)
		StartNextObjective(GameObjectiveOrder[++ObjectiveCounter]);
}


void ASGGameObjectivesHandler::StartNextObjective(EObjectiveType NextObjectiveType)
{
	CurrentObjective = NextObjectiveType;
}


