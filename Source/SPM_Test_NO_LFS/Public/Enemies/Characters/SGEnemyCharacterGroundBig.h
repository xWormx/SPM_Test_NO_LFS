// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGEnemyCharacter.h"
#include "SGEnemyCharacterGroundBig.generated.h"

/**
 * 
 */
UCLASS()
class SPM_TEST_NO_LFS_API ASGEnemyCharacterGroundBig : public ASGEnemyCharacter
{
	GENERATED_BODY()

public:
	ASGEnemyCharacterGroundBig();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	class USGEnemyMeleAttackComponent* MeleeComponent;
};
