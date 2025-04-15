// Fill out your copyright notice in the Description page of Project Settings.


#include "SGEnemyCharacter.h"

#include "SGHealthComponent.h"

// Sets default values
ASGEnemyCharacter::ASGEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	HealthComponent = CreateDefaultSubobject<USGHealthComponent>(TEXT("HealthComponent"));

}

// Called when the game starts or when spawned
void ASGEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (HealthComponent) HealthComponent->OnNoHealth.AddDynamic(this, &ASGEnemyCharacter::HandleDeath);
}

void ASGEnemyCharacter::HandleDeath(float NewHealth)
{
	// Gör andra coola saker
	Destroy();
}

// Called every frame
void ASGEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASGEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

