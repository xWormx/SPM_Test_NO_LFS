// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SGObjectiveToolTipWidget.generated.h"

class UScaleBox;
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
	void DisplayTimer(FText NewTimerText);
	void SetFadeFactor(float NewFadeFactor) { FadeFactor = NewFadeFactor; }
	const bool& GetIsHidden() const { return bIsHidden; }
	const bool& GetTimerAnimationFinished() const  { return bTimerAnimationFinished; }
	void InterruptAndHide() { Hide(); }

protected:
	
	virtual void NativeConstruct() override;

	// Timer
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UScaleBox* ScaleBoxTimer;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* TextTimer;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UOverlay* TimerOverlay;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UImage* TimerImage;
	
	// ToolTp
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UImage* ToolTipBackground;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* ToolTip;


	UPROPERTY(BlueprintReadWrite, Transient, meta=(BindWidgetAnim))
	UWidgetAnimation* ShrinkAndMoveTimer;
	
	UPROPERTY(VisibleAnywhere)
	bool bIsHidden = true;
	
	UPROPERTY(VisibleAnywhere)
	bool bHasFadedOut = false;
	
	UPROPERTY(VisibleAnywhere)
	bool bShouldRender = false;

	UPROPERTY(VisibleAnywhere)
	float CurrentOpacity = 1.0f;
	
	UPROPERTY(VisibleAnywhere, Category = UPROPERTY)
	float FadeFactor = 0.3f;

private:
	bool bTimerAnimationFinished = false;
	FVector2D ScaleBoxTimerFinalPosition;
	FVector2D ScaleBoxTimerFinalSize;
	void Render(float InDeltaTime);
	void SetToolTipText(FText NewToolTip);
	void Hide();

	UFUNCTION()
	void SetScaleBoxTransformAfterAnimation();
	
	FWidgetAnimationDynamicEvent EndTimerAnimation;
};
