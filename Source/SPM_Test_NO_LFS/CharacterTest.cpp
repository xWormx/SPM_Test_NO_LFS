// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterTest.h"

// Sets default values
ACharacterTest::ACharacterTest()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACharacterTest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACharacterTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacterTest::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

