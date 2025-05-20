// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGObjectiveBase.h"
#include "UObject/NoExportTypes.h"
#include "SGObjectiveConfig.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType, EditInlineNew)
class SPM_TEST_NO_LFS_API USGObjectiveConfig : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ASGObjectiveBase> ObjectiveClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString ObjectiveDescriptionToolTip = "Default Objective Description Tooltip!";
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString ObjectiveCompletedToolTip = "Default Mission Completed ToolTip!";

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FString> ObjectiveSubToolTips;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FString> ObjectiveProgressText;
	
	virtual void ApplyData(ASGObjectiveBase* Objective)
	{
		if (!Objective)
			return;

		Objective->SetStartDescriptionTooltipText(ObjectiveDescriptionToolTip);
		Objective->SetCompletedDescriptionTooltipText(ObjectiveCompletedToolTip);
		Objective->AddProgressBarText(ObjectiveProgressText);
		Objective->AddSubTooltips(ObjectiveSubToolTips);
	}
	
};
