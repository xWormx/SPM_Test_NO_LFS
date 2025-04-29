// Fill out your copyright notice in the Description page of Project Settings.


#include "SGObjectiveCollectEmAll.h"

bool ASGObjectiveCollectEmAll::IsCompleted(ASGGameObjectivesHandler* ObjectivesHandler)
{
	return CurrentCollected == CollectGoal;	
}

void ASGObjectiveCollectEmAll::Update(ASGGameObjectivesHandler* ObjectivesHandler)
{
	CurrentCollected++;
}
