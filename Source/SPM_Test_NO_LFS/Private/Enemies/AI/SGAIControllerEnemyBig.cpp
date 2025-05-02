// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/AI/SGAIControllerEnemyBig.h"

#include "Enemies/Characters/SGEnemyCharacter.h"
#include "Enemies/Components/SGEnemyMeleAttackComponent.h"
#include "Kismet/GameplayStatics.h"

ASGAIControllerEnemyBig::ASGAIControllerEnemyBig()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASGAIControllerEnemyBig::BeginPlay()
{
	Super::BeginPlay();
}

void ASGAIControllerEnemyBig::HandleMovement()
{
	if (!AttackTarget)
	{
		return;
	}

	if (!bShouldAlwaysChaseTarget)
	{
		if (!LineOfSightTo(AttackTarget))
		{
			return;
		}
	}

	ControlledCharacter = Cast<ASGEnemyCharacter>(GetPawn());

	if (CanAttackTarget())
	{
		if (ControlledCharacter)
		{
			if (USGEnemyMeleAttackComponent* MeleeComponent = ControlledCharacter->FindComponentByClass<USGEnemyMeleAttackComponent>())
			{
				MeleeComponent->StartAttack(AttackTarget);
			}
		}
		if (!ControlledCharacter)
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, TEXT("No Controlled Character"));
			}
		}
	}
	else
	{
		MoveToActor(AttackTarget);
	}
}

void ASGAIControllerEnemyBig::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	HandleMovement();
}
