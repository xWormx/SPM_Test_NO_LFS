// Fill out your copyright notice in the Description page of Project Settings.


#include "SGObjectiveCollectEmAll.h"

bool ASGObjectiveCollectEmAll::CheckProgress()
{
	return CurrentCollected == CollectGoal;	
}

void ASGObjectiveCollectEmAll::Update()
{
	CurrentCollected++;
}
