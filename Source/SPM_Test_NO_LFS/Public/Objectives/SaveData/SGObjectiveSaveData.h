// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SGObjectiveSaveData.generated.h"

/**
 * 
 */
UCLASS()
class SPM_TEST_NO_LFS_API USGObjectiveSaveData : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<FString> ObjectiveProgressText;
	
	UPROPERTY()
	int32 CurrentSubObjectiveStep;
	
	UPROPERTY()
	FString ObjectiveDescriptionToolTip;
	
	UPROPERTY()
	FString ObjectiveCompletedToolTip;
	
	UPROPERTY()
	FString CurrentSubToolTip;
	
	UPROPERTY()
	TArray<FString> ObjectiveSubToolTips;
};
