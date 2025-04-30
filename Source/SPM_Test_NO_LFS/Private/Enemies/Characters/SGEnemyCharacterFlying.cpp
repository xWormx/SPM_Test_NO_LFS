// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Characters/SGEnemyCharacterFlying.h"

#include "Enemies/Components/SGEnemyChargeAttackComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/Core/Analytics/NetStatsUtils.h"

ASGEnemyCharacterFlying::ASGEnemyCharacterFlying()
{
	PrimaryActorTick.bCanEverTick = true;

	ChargeAttackComponent = CreateDefaultSubobject<USGEnemyChargeAttackComponent>(TEXT("ChargeAttackComponent"));

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
