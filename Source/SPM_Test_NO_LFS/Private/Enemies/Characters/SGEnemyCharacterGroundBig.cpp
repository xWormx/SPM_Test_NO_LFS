// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Characters/SGEnemyCharacterGroundBig.h"

#include "Enemies/Components/SGEnemyMeleeAttackComponent.h"

ASGEnemyCharacterGroundBig::ASGEnemyCharacterGroundBig()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.5f;
	AttackComponent = CreateDefaultSubobject<USGEnemyMeleeAttackComponent>(TEXT("MeleeComponent"));
}

/*void ASGEnemyCharacterGroundBig::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}*/

void ASGEnemyCharacterGroundBig::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ASGEnemyCharacterGroundBig::BeginPlay()
{
	Super::BeginPlay();
}
