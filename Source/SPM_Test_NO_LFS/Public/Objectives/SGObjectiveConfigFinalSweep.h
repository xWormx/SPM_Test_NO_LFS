// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGObjectiveFinalSweep.h"
#include "Objectives/SGObjectiveConfig.h"
#include "SGObjectiveConfigFinalSweep.generated.h"

/**
 * 
 */
UCLASS()
class SPM_TEST_NO_LFS_API USGObjectiveConfigFinalSweep : public USGObjectiveConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FVector EscapeTriggerZoneLocation;

	virtual void ApplyData(ASGObjectiveBase* Objective) override
	{
		Super::ApplyData(Objective);

		if (ASGObjectiveFinalSweep* FinalSweep = Cast<ASGObjectiveFinalSweep>(Objective))
		{
			FinalSweep->SetActorLocation(EscapeTriggerZoneLocation);
		}
	}
	
};
