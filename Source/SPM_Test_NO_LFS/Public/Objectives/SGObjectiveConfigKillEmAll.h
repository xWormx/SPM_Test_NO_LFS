/*

Written by
----------------------
- Carl-Johan Larson Eliasson
- cael7567
	
*/
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
