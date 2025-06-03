/*

Written by
----------------------
- Carl-Johan Larson Eliasson
- cael7567
	
*/

#pragma once

#include "CoreMinimal.h"
#include "SGObjectiveBase.h"
#include "Blueprint/UserWidget.h"
#include "SGObjectiveToolTipWidget.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDifficultyChanged, int, NewDifficultLevel);

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
struct FHorizontalBoxList
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<USGHorizontalBoxObjective*> HorizontalBoxElements;
};

UCLASS()
class SPM_TEST_NO_LFS_API USGObjectiveToolTipWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	
	//void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void Display(FText NewToolTip);
	void DeInitialize();
	const bool& GetTimerAnimationFinished() const  { return bTimerAnimationFinished; }

	void PauseAllOngoingAnimations();
	void ResumeAllOngoingAnimations();
	void ShowVisitTerminal();
	void HideVisitTerminal();
	void SetMissionAlertText(FString NewText);
	void PlayEscapeWithPodAnimation(FString TextToDisplay);
	void ShowMissionVerticalBox();
	void HideMissionVerticalBox();
	void ShowToolTipScaleBox();
	void HideToolTipScaleBox();
	
	USGHorizontalBoxObjective* CreateProgressTextElement(int32 ObjectiveID, FText KeyText, FText ValueText);
	USGHorizontalBoxObjective* CreateProgressTextElementCompleted(FText KeyText, FText ValueText);
	void AddHorizontalBoxToMap(int32 ObjectiveID, USGHorizontalBoxObjective* NewWidget);
	
	// TODO (Calle): Denna kan returnera mapen istället med CurrentObjective->ID och välj element i Value-arrayen mha CurrentSubStep??
	USGHorizontalBoxObjective* GetCurrentHorizontalBoxObjective();
	USGHorizontalBoxObjective* GetHorizontalBoxObjective(ASGObjectiveBase* Objective, int32 BoxIndex);
	
	// NOTE (Calle): Test funktion för att ta bort HBox element i progresswindow
	void ClearProgressWindowElements()
	{
		HorizontalBoxObjectiveMap.Empty();
	}
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
	TMap<int32, FHorizontalBoxList> HorizontalBoxObjectiveMap;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USGHorizontalBoxObjective* CurrentHorizontalBoxObjectiveElement;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UVerticalBox* VerticalBoxMission;
	
	//Animations
	UPROPERTY(BlueprintReadWrite, Transient, meta=(BindWidgetAnim))
	UWidgetAnimation* AnimationToolTipOutOfWindow;
	float AnimationToolTipOutOfWindowPauseTime;
	bool bAnimationToolTipOutOfWindowWasPaused;

	UPROPERTY(BlueprintReadWrite, Transient, meta=(BindWidgetAnim))
	UWidgetAnimation* AnimationVisitTerminal;
	float AnimationVisitTerminalPauseTime;
	bool bAnimationVisitTerminalWasPaused;

	UPROPERTY(BlueprintReadWrite, Transient, meta=(BindWidgetAnim))
	UWidgetAnimation* AnimationEscapeWithPod;

	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	USoundBase* TextClickSound;
	
	FTimerHandle CharByCharTimer;
	int32 CharIndex = 0;
	FString TextCharByChar = "123456DONE!";

private:
	bool bTimerAnimationFinished = false;
	FVector2D ScaleBoxTimerFinalPosition;
	FVector2D ScaleBoxTimerFinalSize;
	
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


