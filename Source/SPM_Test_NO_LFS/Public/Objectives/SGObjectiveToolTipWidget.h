// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SGObjectiveToolTipWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDifficultyChanged, int, NewDifficultLevel);

class UOverlay;
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
	
	const bool& GetTimerAnimationFinished() const  { return bTimerAnimationFinished; }

	void PauseAllOngoingAnimations();
	void ResumeAllOngoingAnimations();
	void ShowVisitTerminal();
	void HideVisitTerminal();
	void ShowMissionVerticalBox();
	void HideMissionVerticalBox();
	void ShowToolTipScaleBox();
	void HideToolTipScaleBox();
	
	void AddProgressTextElement(FText KeyText, FText ValueText);
	USGHorizontalBoxObjective* CreateProgressTextElement(FText KeyText, FText ValueText);
	USGHorizontalBoxObjective* GetHorizontalBoxAtIndex(int32 index);
	USGHorizontalBoxObjective* GetCurrentHorizontalBoxObjective() { return CurrentHorizontalBoxObjectiveElement; }
	TArray<USGHorizontalBoxObjective*> GetHorizontalBoxObjectiveList() {return HorizontalObjectiveList; }

	FOnDifficultyChanged OnDifficultyChanged;
protected:
	
	virtual void NativeConstruct() override;

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
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UOverlay* OverlayVisitTerminal;
	
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
	UPROPERTY(VisibleAnywhere)
	int DifficultLevel = 0;
	
	//Animations
	UPROPERTY(BlueprintReadWrite, Transient, meta=(BindWidgetAnim))
	UWidgetAnimation* AnimationToolTipOutOfWindow;
	float AnimationToolTipOutOfWindowPauseTime;
	bool bAnimationToolTipOutOfWindowWasPaused;

	UPROPERTY(BlueprintReadWrite, Transient, meta=(BindWidgetAnim))
	UWidgetAnimation* AnimationVisitTerminal;
	float AnimationVisitTerminalPauseTime;
	bool bAnimationVisitTerminalWasPaused;

	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	USoundBase* TextClickSound;
	
	FTimerHandle CharByCharTimer;
	int32 CharIndex = 0;
	FString TextCharByChar = "123456DONE!";

private:
	bool bTimerAnimationFinished = false;
	FVector2D ScaleBoxTimerFinalPosition;
	FVector2D ScaleBoxTimerFinalSize;
	void UpdateDifficultyBar(float InDeltaTime);
	void SetToolTipText(FText NewToolTip);
	

	UFUNCTION()
	void DisplayCharByChar(const FString& StringToolTip);
	UFUNCTION()
	void OnEndMoveToolTipAnimation();
	UFUNCTION()
	void OnEndHideToolTipAnimation();
	
	FWidgetAnimationDynamicEvent EndTimerAnimation;
	FWidgetAnimationDynamicEvent EndMoveToolTipToProgressWindowAnimation;
	FWidgetAnimationDynamicEvent EndHideToolTipAnimation;
};


