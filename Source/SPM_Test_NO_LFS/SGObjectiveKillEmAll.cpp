// Fill out your copyright notice in the Description page of Project Settings.


#include "SGObjectiveKillEmAll.h"

bool ASGObjectiveKillEmAll::CheckProgress()
{
	return CurrentEnemiesKilled == EnemiesToKill;
}

void ASGObjectiveKillEmAll::Update()
{
	CurrentEnemiesKilled++;
}
