// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGObjectiveBase.h"
#include "SGObjectiveKillEmAll.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SPM_TEST_NO_LFS_API ASGObjectiveKillEmAll : public ASGObjectiveBase
{
	GENERATED_BODY()
public:
	virtual bool CheckProgress() override;
	virtual void Update() override;
private:
	UPROPERTY(EditAnywhere, Category=UPROPERTY)
	unsigned int EnemiesToKill;

	UPROPERTY(EditAnywhere ,Category=UPROPERTY)
	unsigned int CurrentEnemiesKilled = 0;
};
