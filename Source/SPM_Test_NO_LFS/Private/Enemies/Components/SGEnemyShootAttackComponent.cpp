// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Components/SGEnemyShootAttackComponent.h"

#include "Enemies/Components/SGEnemyProjectile.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values for this component's properties
USGEnemyShootAttackComponent::USGEnemyShootAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void USGEnemyShootAttackComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void USGEnemyShootAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                 FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USGEnemyShootAttackComponent::StartAttack(AActor* Target)
{
	if (!Target || !bCanAttack || !OwnerCharacter)
	{
		return;
	}
	Super::StartAttack(Target);
	PerformAttack(Target);
}

void USGEnemyShootAttackComponent::PerformAttack(AActor* Target)
{

	if (!ProjectileClass)
	{
		return;
	}

    FVector SpawnLocation = OwnerCharacter->GetActorLocation() + OwnerCharacter->GetActorForwardVector() * 20.f;
	
    FVector Direction = (Target->GetActorLocation() - SpawnLocation).GetSafeNormal();
    FRotator SpawnRotation = Direction.Rotation();

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = OwnerCharacter;
    SpawnParams.Instigator = OwnerCharacter;
	
    GetWorld()->SpawnActor<ASGEnemyProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
	
}

