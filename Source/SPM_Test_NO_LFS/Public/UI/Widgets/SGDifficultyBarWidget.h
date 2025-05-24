// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Objectives/SGGameObjectivesHandler.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "SGDifficultyBarWidget.generated.h"

class UImage;
class UOverlay;

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

	UPROPERTY(VisibleAnywhere, Category="UProperty - Difficulty Bar")
	USGObjectiveToolTipWidget* ObjectiveToolTipWidget;
	
	void MoveOverlaysLeft(float TranslationMovement);
	float GetTriggerAbsolutePositionX() const;
	float GetDifficultBarScrollSpeed() const;
	TArray<UOverlay*> GetOverlays();

protected:
	virtual void NativeConstruct() override;
	void ChangeDifficulty(int32 NextDifficultyLevel);
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	bool HasReachedMaxDifficultyPosition();
	bool LastDifficultNotReached();
	bool HasReachedMaxDifficulty() const;
	bool LastDifficultBoxNotCenteredAtTrigger();

public:
	void UpdateDifficultyBar(float InDeltaTime);
	
private:
	UPROPERTY(VisibleAnywhere)
	TArray<UOverlay*> Overlays;

	UPROPERTY(VisibleAnywhere, Transient, Category="UProperty - Difficulty Bar", meta = (BindWidgetAnim))
	UWidgetAnimation* AnimationTriggerNewDifficultLevel;

	UPROPERTY(VisibleAnywhere, Category="UProperty - Difficulty Bar", Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* AnimationShowWarningMessage;

	// How fast the difficultbar should scroll
	UPROPERTY(EditAnywhere, Category="UProperty - Difficulty Bar")
	float DifficultBarScrollSpeed = 5.f;

	UPROPERTY(EditAnywhere, Category="UProperty - Difficulty Bar")
	float DifficultBoxStartPosition = 550.f; // 800;

	UFUNCTION()
	void AnimationTriggerAlert(int NewDifficultLevel);
	
	UPROPERTY(EditAnywhere, Category="UProperty - Difficulty Bar")
	USoundBase* SoundWarningDifficultLevel; ///Script/Engine.SoundWave'/Game/Sounds/Warning/Warning.Warning'

	UPROPERTY(EditAnywhere, Category="UProperty - Difficulty Bar")
	USoundBase* SoundAlarmBell; ///Script/Engine.SoundWave'/Game/Sounds/Warning/AlarmBell.AlarmBell'

	UPROPERTY(VisibleAnywhere, Category="UProperty - Difficulty Bar", meta=(ToolTip="Updated during gameplay"))
	float DifficultyBarOffsetLeft = 0.0f;

	UPROPERTY(VisibleAnywhere, Category="UProperty - Difficulty Bar")
	int DifficultLevel = 0;

	UPROPERTY(VisibleAnywhere, Category="UProperty - Difficulty Bar")
	float TriggerPosition = 0.0f;
};
