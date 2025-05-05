// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Characters/SGEnemyCharacterFlying.h"

#include "Components/CapsuleComponent.h"
#include "Enemies/Components/SGEnemyChargeAttackComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ASGEnemyCharacterFlying::ASGEnemyCharacterFlying()
{
	PrimaryActorTick.bCanEverTick = true;

	AttackComponent = CreateDefaultSubobject<USGEnemyChargeAttackComponent>(TEXT("ChargeAttackComponent"));

	GetCharacterMovement()->DefaultLandMovementMode = MOVE_Flying;
	
	GetCharacterMovement()->SetMovementMode(MOVE_Flying);

	GetCharacterMovement()->MaxFlySpeed = 400.f;

	GetMesh()->SetEnableGravity(false);
}

void ASGEnemyCharacterFlying::BeginPlay()
{
	Super::BeginPlay();
}

void ASGEnemyCharacterFlying::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASGEnemyCharacterFlying::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
