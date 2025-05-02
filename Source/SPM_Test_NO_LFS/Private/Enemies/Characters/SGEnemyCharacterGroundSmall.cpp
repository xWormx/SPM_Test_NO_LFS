// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Characters/SGEnemyCharacterGroundSmall.h"

#include "Enemies/Components/SGEnemyShootAttackComponent.h"

ASGEnemyCharacterGroundSmall::ASGEnemyCharacterGroundSmall()
{
	PrimaryActorTick.bCanEverTick = true;

	ShootAttackComponent = CreateDefaultSubobject<USGEnemyShootAttackComponent>(TEXT("ShootAttackComponent"));
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
