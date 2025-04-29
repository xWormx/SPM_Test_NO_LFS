// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Characters/SGEnemyCharacterGroundSmall.h"

ASGEnemyCharacterGroundSmall::ASGEnemyCharacterGroundSmall()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASGEnemyCharacterGroundSmall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASGEnemyCharacterGroundSmall::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ASGEnemyCharacterGroundSmall::BeginPlay()
{
	Super::BeginPlay();
}
