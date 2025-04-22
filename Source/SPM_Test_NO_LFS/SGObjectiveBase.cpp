// Fill out your copyright notice in the Description page of Project Settings.


#include "SGObjectiveBase.h"

// Sets default values
ASGObjectiveBase::ASGObjectiveBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ASGObjectiveBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASGObjectiveBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASGObjectiveBase::ActivateObjective()
{
	bIsActivated = true;
}

