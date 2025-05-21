// Fill out your copyright notice in the Description page of Project Settings.
// comment
#pragma once

#include "CoreMinimal.h"
#include "SGObjectiveDefendThePod.h"
#include "SGObjectivePodArrival.h"
#include "Objectives/SGObjectiveConfig.h"
#include "SGObjectiveConfigPodArrival.generated.h"

/**
 * 
 */
UCLASS()
class SPM_TEST_NO_LFS_API USGObjectiveConfigPodArrival : public USGObjectiveConfig
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	float SecondsToWaitForPodArrival;

	UPROPERTY(EditAnywhere)
	FVector LandingZoneLocation;

	virtual void ApplyData(ASGObjectiveBase* Objective) override
	{
		Super::ApplyData(Objective);
		if (ASGObjectivePodArrival* WaitForPod = Cast<ASGObjectivePodArrival>(Objective))
		{
			WaitForPod->SetWaitForPodTimeInSeconds(SecondsToWaitForPodArrival);
			WaitForPod->SetActorLocation(LandingZoneLocation);
		}
	}
};
