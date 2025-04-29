// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGAIControllerEnemyBase.h"
#include "SGAIControllerEnemyBig.generated.h"

/**
 * 
 */
UCLASS()
class SPM_TEST_NO_LFS_API ASGAIControllerEnemyBig : public ASGAIControllerEnemyBase
{
	GENERATED_BODY()

protected:
	ASGAIControllerEnemyBig();

	virtual void BeginPlay() override;

	virtual void HandleMovement() override;

public:
	virtual void Tick(float DeltaTime) override;
	
};
