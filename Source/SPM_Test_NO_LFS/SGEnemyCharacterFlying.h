// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGEnemyCharacter.h"
#include "SGEnemyCharacterFlying.generated.h"

/**
 * 
 */
UCLASS()
class SPM_TEST_NO_LFS_API ASGEnemyCharacterFlying : public ASGEnemyCharacter
{
	GENERATED_BODY()

public:
	ASGEnemyCharacterFlying();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void FlyTowradsPlayer();

	UPROPERTY(EditAnywhere, Category = "AI" ,meta = (AllowPrivateAccess = true))
	float DetectionRadius = 1000.f;

	FVector StartLocation;

	float HoverAmplitude = 50.f;
	float HoverSpeed = 1.5f;

	UPROPERTY(EditAnywhere, Category = "AI", meta = (AllowPrivateAccess = true))
	AActor* Target;
	
	
};
