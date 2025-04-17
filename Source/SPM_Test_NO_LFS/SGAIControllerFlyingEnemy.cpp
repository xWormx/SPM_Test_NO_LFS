// Fill out your copyright notice in the Description page of Project Settings.


#include "SGAIControllerFlyingEnemy.h"

#include "Kismet/GameplayStatics.h"

void ASGAIControllerFlyingEnemy::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

void ASGAIControllerFlyingEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
