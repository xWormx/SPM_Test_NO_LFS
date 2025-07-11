/*

Written by
----------------------
- Carl-Johan Larson Eliasson
- cael7567
	
*/
#pragma once

#include "CoreMinimal.h"
#include "Objectives/SGObjectivePodArrival.h"
#include "SGObjectiveConfig.h"
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
