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

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;


protected:
	UPROPERTY(EditAnywhere)
	class USGEnemyChargeAttackComponent* ChargeAttackComponent;
};
