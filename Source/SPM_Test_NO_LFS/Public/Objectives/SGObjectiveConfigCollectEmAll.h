/*

Written by
----------------------
- Carl-Johan Larson Eliasson
- cael7567
	
*/
#pragma once

#include "CoreMinimal.h"
#include "SGObjectiveCollectEmAll.h"
#include "Objectives/SGObjectiveConfig.h"
#include "SGObjectiveConfigCollectEmAll.generated.h"

/**
 * 
 */
UCLASS()
class SPM_TEST_NO_LFS_API USGObjectiveConfigCollectEmAll : public USGObjectiveConfig
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	int CollectGoal;

	virtual void ApplyData(ASGObjectiveBase* Objective) override
	{
		Super::ApplyData(Objective);
		if (ASGObjectiveCollectEmAll* CollectEmAll = Cast<ASGObjectiveCollectEmAll>(Objective))
		{
			CollectEmAll->SetNumberToCollect(CollectGoal);
		}
	}
};
