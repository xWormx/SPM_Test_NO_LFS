// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objectives/SGObjectiveBase.h"
#include "SGObjectiveFinalSweep.generated.h"

/**
 * 
 */
UCLASS()
class SPM_TEST_NO_LFS_API ASGObjectiveFinalSweep : public ASGObjectiveBase, public ISGObjectiveInterface
{
	GENERATED_BODY()
	
	virtual bool IsCompleted() override;
	virtual void OnStart() override;
	virtual void OnCompleted() override;
	virtual void Update() override;
	virtual EObjectiveType GetObjectiveType() { return EObjectiveType::EOT_FinalSweep; }

	void IncrementEnemiesKilled();

private:
	UPROPERTY(VisibleAnywhere, Category = "UPROPERTY", Meta = (AllowPrivateAccess = true))
	uint32 CurrentEnemiesKilled;
	
};
