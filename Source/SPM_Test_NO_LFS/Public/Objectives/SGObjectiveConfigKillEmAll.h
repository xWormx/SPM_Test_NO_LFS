// Fill out your copyright notice in the Description page of Project Settings.
// comment
#pragma once

#include "CoreMinimal.h"
#include "SGObjectiveKillEmAll.h"
#include "Objectives/SGObjectiveConfig.h"
#include "SGObjectiveConfigKillEmAll.generated.h"

/**
 * 
 */
UCLASS()
class SPM_TEST_NO_LFS_API USGObjectiveConfigKillEmAll : public USGObjectiveConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int EnemiesToKill;

	virtual void ApplyData(ASGObjectiveBase* Objective) override
	{
		Super::ApplyData(Objective);
		if (ASGObjectiveKillEmAll* KillEmAll = Cast<ASGObjectiveKillEmAll>(Objective))
		{
			KillEmAll->SetEnemiesToKill(EnemiesToKill);
		}
	}
};
