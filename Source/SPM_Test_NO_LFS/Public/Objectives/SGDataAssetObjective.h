/*
 
Written by
----------------------
- Carl-Johan Larson Eliasson
- cael7567

*/

#pragma once

#include "CoreMinimal.h"
#include "SGObjectiveBase.h"
#include "Objectives/Config/SGObjectiveConfig.h"
#include "Engine/DataAsset.h"
#include "SGDataAssetObjective.generated.h"

/**
 * 
 */

UCLASS()
class SPM_TEST_NO_LFS_API USGDataAssetObjective : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Instanced, Category = "Objectives")
	TArray<USGObjectiveConfig*> ObjectiveConfigs;

	
	
};
