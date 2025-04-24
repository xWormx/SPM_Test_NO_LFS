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
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	void Display(FText NewToolTip);
	bool GetIsHidden() { return bIsHidden; }
	void InterruptAndHide() { Hide(); }

protected:
	
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* ToolTip;

	UPROPERTY(VisibleAnywhere)
	bool bIsHidden = true;
	
	UPROPERTY(VisibleAnywhere)
	bool bHasFadedOut = false;
	
	UPROPERTY(VisibleAnywhere)
	bool bShouldRender = false;

	UPROPERTY(VisibleAnywhere)
	float CurrentOpacity = 1.0f;
	
	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	float FadeFactor = 0.3f;

private:
	void Render(float InDeltaTime);
	void SetToolTipText(FText NewToolTip);
	void Hide();

};
