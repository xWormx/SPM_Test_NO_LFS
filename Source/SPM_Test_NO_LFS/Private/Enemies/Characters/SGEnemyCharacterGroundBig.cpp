// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Characters/SGEnemyCharacterGroundBig.h"

#include "Enemies/Components/SGEnemyMeleAttackComponent.h"

ASGEnemyCharacterGroundBig::ASGEnemyCharacterGroundBig()
{
	PrimaryActorTick.bCanEverTick = true;

	MeleeComponent = CreateDefaultSubobject<USGEnemyMeleAttackComponent>(TEXT("MeleeComponent"));
}

void ASGEnemyCharacterGroundBig::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASGEnemyCharacterGroundBig::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ASGEnemyCharacterGroundBig::BeginPlay()
{
	Super::BeginPlay();
}
