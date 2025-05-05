// Fill out your copyright notice in the Description page of Project Settings.


#include "Objectives/SGObjectiveCollectEmAll.h"

bool ASGObjectiveCollectEmAll::IsCompleted(ASGGameObjectivesHandler* ObjectivesHandler)
{
	return CurrentCollected == CollectGoal;	
}

void ASGObjectiveCollectEmAll::Update(ASGGameObjectivesHandler* ObjectivesHandler)
{
	CurrentCollected++;
	FString StrCollected = FString::Printf(TEXT("FUELRODS: %d/%d"), CurrentCollected, CollectGoal);
	SetCurrentProgressText(StrCollected);
}
