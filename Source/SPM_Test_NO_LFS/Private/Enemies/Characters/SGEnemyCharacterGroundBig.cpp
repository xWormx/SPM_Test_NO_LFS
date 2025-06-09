// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Characters/SGEnemyCharacterGroundBig.h"

#include "Enemies/Components/SGEnemyMeleeAttackComponent.h"

ASGEnemyCharacterGroundBig::ASGEnemyCharacterGroundBig()
{
	PrimaryActorTick.bCanEverTick = true;
	AttackComponent = CreateDefaultSubobject<USGEnemyMeleeAttackComponent>(TEXT("MeleeComponent"));
}

void ASGEnemyCharacterGroundBig::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ASGEnemyCharacterGroundBig::BeginPlay()
{
	Super::BeginPlay();
}
