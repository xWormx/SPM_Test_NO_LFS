// Fill out your copyright notice in the Description page of Project Settings.


#include "SGGroundEnemyCharacter.h"

ASGGroundEnemyCharacter::ASGGroundEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASGGroundEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASGGroundEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ASGGroundEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
}
