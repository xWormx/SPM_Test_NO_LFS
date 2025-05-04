// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Components/SGEnemyAttackComponentBase.h"

#include "Enemies/Characters/SGEnemyCharacter.h"


// Sets default values for this component's properties
USGEnemyAttackComponentBase::USGEnemyAttackComponentBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bCanAttack = true;
	OwnerCharacter = Cast<ASGEnemyCharacter>(GetOwner());
}


// Called when the game starts
void USGEnemyAttackComponentBase::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USGEnemyAttackComponentBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USGEnemyAttackComponentBase::StartAttack(AActor* Target)
{
	if (!Target || !bCanAttack || !OwnerCharacter)
	{
		return;
	}

	bCanAttack = false;

	GetWorld()->GetTimerManager().SetTimer(
		CooldownTimerHandle,
		this,
		&USGEnemyAttackComponentBase::ResetAttackCooldown,
		AttackCooldown,
		false
	);
}

void USGEnemyAttackComponentBase::PerformAttack(AActor* Target)
{
	
}

void USGEnemyAttackComponentBase::ResetAttackCooldown()
{
	bCanAttack = true;
}

