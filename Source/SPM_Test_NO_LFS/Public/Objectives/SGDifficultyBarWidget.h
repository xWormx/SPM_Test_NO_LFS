// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGGameObjectivesHandler.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "SGDifficultyBarWidget.generated.h"

class UImage;
class UOverlay;
/**
 * 
 */
UCLASS()
class SPM_TEST_NO_LFS_API USGDifficultyBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UOverlay* OverlayDifficulty;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UOverlay* OverlayDifficulty_1;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UOverlay* OverlayDifficulty_2;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UOverlay* OverlayDifficulty_3;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UOverlay* OverlayWarningMessage;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* ImageTrigger;

	UPROPERTY(VisibleAnywhere, Category=UPROPERTY)
	USGObjectiveToolTipWidget* ObjectiveToolTipWidget;
	
	void MoveOverlaysLeft(float TranslationMovement);
	float GetTriggerAbsolutePositionX() { return ImageTrigger->GetCachedGeometry().GetAbsolutePosition().X; }
	float GetDifficultBarScrollSpeed() { return DifficultBarScrollSpeed; }
	TArray<UOverlay*> GetOverlays() { return Overlays; }
protected:
	virtual void NativeConstruct() override;
private:
	UPROPERTY(VisibleAnywhere)
	TArray<UOverlay*> Overlays;

	UPROPERTY(VisibleAnywhere, Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* AnimationTriggerNewDifficultLevel;

	UPROPERTY(VisibleAnywhere, Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* AnimationShowWarningMessage;

	// How fast the difficultbar should scroll
	UPROPERTY(EditAnywhere)
	float DifficultBarScrollSpeed;
	UPROPERTY(EditAnywhere)
	float DifficultBoxStartPosition = 800;

	UFUNCTION()
	void AnimationTriggerAlert(int NewDifficultLevel);

};
