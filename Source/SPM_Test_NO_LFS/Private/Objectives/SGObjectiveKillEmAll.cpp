// Fill out your copyright notice in the Description page of Project Settings.


#include "Objectives/SGObjectiveKillEmAll.h"

#include "Objectives/SGGameObjectivesHandler.h"

bool ASGObjectiveKillEmAll::IsCompleted(ASGGameObjectivesHandler* ObjectivesHandler)
{
	return CurrentEnemiesKilled == EnemiesToKill;
}

void ASGObjectiveKillEmAll::Update(ASGGameObjectivesHandler* ObjectivesHandler)
{
	CurrentEnemiesKilled++;
	FString StrEnemiesKilled = FString::Printf(TEXT("ENEMIES KILLED: %d/%d"), CurrentEnemiesKilled, EnemiesToKill);
	SetCurrentProgressText(StrEnemiesKilled);

}
