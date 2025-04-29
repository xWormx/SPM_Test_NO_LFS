// Fill out your copyright notice in the Description page of Project Settings.


#include "SGObjectiveKillEmAll.h"

#include "SGGameObjectivesHandler.h"

bool ASGObjectiveKillEmAll::IsCompleted(ASGGameObjectivesHandler* ObjectivesHandler)
{
	return CurrentEnemiesKilled == EnemiesToKill;
}

void ASGObjectiveKillEmAll::Update(ASGGameObjectivesHandler* ObjectivesHandler)
{
	CurrentEnemiesKilled++;
}
