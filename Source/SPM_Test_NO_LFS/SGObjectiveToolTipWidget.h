// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SGObjectiveToolTipWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPM_TEST_NO_LFS_API USGObjectiveToolTipWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetToolTipText(FText NewToolTip);
protected:
	
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* ToolTip;

};
