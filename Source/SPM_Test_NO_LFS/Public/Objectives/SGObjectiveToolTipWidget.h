// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SGObjectiveToolTipWidget.generated.h"

class UTextBlock;
class UImage;
class USGDifficultyBarWidget;
class UVerticalBox;
class USGHorizontalBoxObjective;
class ASGObjectiveBase;
class UScaleBox;
/**
 * 
 */

USTRUCT(BlueprintType)
struct FProgressText
{
	GENERATED_BODY();
	
	UPROPERTY(EditAnywhere)
	TArray<FString> SubText;	
};

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

	void ShowVisitTerminal();
	void HideVisitTerminal();
	void ShowMissionVerticalBox();
	void HideMissionVerticalBox();
	void ShowToolTipScaleBox();
	void HideToolTipScaleBox();
	
	void InterruptAndHide() { Hide(); }
	void AddProgressTextElement(FText KeyText, FText ValueText);
	USGHorizontalBoxObjective* CreateProgressTextElement(FText KeyText, FText ValueText);
	USGHorizontalBoxObjective* GetHorizontalBoxAtIndex(int32 index);
	USGHorizontalBoxObjective* GetCurrentHorizontalBoxObjective() { return CurrentHorizontalBoxObjectiveElement; } 
protected:
	
	virtual void NativeConstruct() override;

	// Timer
	/*UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UScaleBox* ScaleBoxTimer;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* TextTimer;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UOverlay* TimerOverlay;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UImage* TimerImage;
	*/
	// ToolTip
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UScaleBox* ScaleBoxToolTip;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UImage* ToolTipBackground;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* ToolTip;

	// Visit Terminal Text
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* TextBlockVisitTerminal;
	
	// ProgressWindow NEW
	UPROPERTY(EditAnywhere)
	TSubclassOf<USGHorizontalBoxObjective> HorizontalBoxObjectiveClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<USGHorizontalBoxObjective*> HorizontalObjectiveList;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USGHorizontalBoxObjective* CurrentHorizontalBoxObjectiveElement;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UVerticalBox* VerticalBoxMission;

	// Difficulty Progress Bar
	UPROPERTY(EditAnywhere)
	TSubclassOf<USGDifficultyBarWidget> DifficultyBarWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USGDifficultyBarWidget* DifficultyBarWidget;
	UPROPERTY(VisibleAnywhere)
	float DifficultyBarOffsetLeft = 0.0f;
	
	//Animations
	UPROPERTY(BlueprintReadWrite, Transient, meta=(BindWidgetAnim))
	UWidgetAnimation* AnimationToolTipOutOfWindow;
	UPROPERTY(BlueprintReadWrite, Transient, meta=(BindWidgetAnim))
	UWidgetAnimation* AnimationVisitTerminal;

	
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

	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	USoundBase* TextClickSound;
	
	FTimerHandle CharByCharTimer;
	int32 CharIndex = 0;
	FString TextCharByChar = "123456DONE!";

private:
	bool bTimerAnimationFinished = false;
	FVector2D ScaleBoxTimerFinalPosition;
	FVector2D ScaleBoxTimerFinalSize;
	void Render(float InDeltaTime);
	void Hide();
	void SetToolTipText(FText NewToolTip);
	

	UFUNCTION()
	void DisplayCharByChar(const FString& StringToolTip);
	UFUNCTION()
	void SetScaleBoxTransformAfterAnimation();	
	UFUNCTION()
	void OnEndMoveToolTipAnimation();
	UFUNCTION()
	void OnEndHideToolTipAnimation();
	
	FWidgetAnimationDynamicEvent EndTimerAnimation;
	FWidgetAnimationDynamicEvent EndMoveToolTipToProgressWindowAnimation;
	FWidgetAnimationDynamicEvent EndHideToolTipAnimation;
};



