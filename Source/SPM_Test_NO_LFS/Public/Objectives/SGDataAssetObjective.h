// Fill out your copyright notice in the Description page of Project Settings.
// comment
#pragma once

#include "CoreMinimal.h"
#include "SGObjectiveBase.h"
#include "SGObjectiveConfig.h"
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
