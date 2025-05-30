/*

Written by
----------------------
- Carl-Johan Larson Eliasson
- cael7567
	
*/

#pragma once

#include "CoreMinimal.h"
#include "../../Objectives/SGGameObjectivesHandler.h"
#include "Blueprint/UserWidget.h"
#include "SGDifficultyBarWidget.generated.h"

class UImage;
class UOverlay;

UCLASS()
class SPM_TEST_NO_LFS_API USGDifficultyBarWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void PauseDifficultyBar(bool bPauseBar);
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void UpdateDifficultyBar(float InDeltaTime);
	void ChangeDifficulty(int32 NextDifficultyLevel);

	bool LastDifficultNotReached();
	bool LastDifficultBoxNotCenteredAtTrigger() const;
	float GetTriggerAbsolutePositionX() const;

private:
	UPROPERTY(EditAnywhere, Category="UProperty - Difficulty Bar")
	bool bPause = false;

	UPROPERTY(EditAnywhere, Category="UProperty - Difficulty Bar", meta=(ToolTip="How fast the difficultybar should scroll"))
	float DifficultBarScrollSpeed = 5.f;

	UPROPERTY(EditAnywhere, Category="UProperty - Difficulty Bar")
	float DifficultBoxStartPosition = 550.f; // 800;

	UPROPERTY(VisibleAnywhere, Category="UProperty - Difficulty Bar", meta=(ToolTip="Updated during gameplay"))
	float DifficultyBarOffsetLeft = 0.0f;

	UPROPERTY(VisibleAnywhere, Category="UProperty - Difficulty Bar")
	int CurrentDifficultyLevel = 0;

//----- OVERLAYS
	UPROPERTY(VisibleAnywhere, Category="UProperty - Difficulty Bar", meta=(ToolTip="Updated during gameplay"))
	TArray<UOverlay*> Overlays;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UOverlay* OverlayDifficulty;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UOverlay* OverlayDifficulty_1;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UOverlay* OverlayDifficulty_2;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UOverlay* OverlayDifficulty_3;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UOverlay* OverlayWarningMessage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UImage* ImageTrigger;

//----- ANIMATION
	UPROPERTY(VisibleAnywhere, Transient, Category="UProperty - Difficulty Bar", meta = (BindWidgetAnim))
	UWidgetAnimation* AnimationTriggerNewDifficultLevel;

	UPROPERTY(VisibleAnywhere, Category="UProperty - Difficulty Bar", Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* AnimationShowWarningMessage;

//----- SOUND
	UPROPERTY(EditAnywhere, Category="UProperty - Difficulty Bar")
	USoundBase* SoundWarningDifficultLevel;//Script/Engine.SoundWave'/Game/Sounds/Warning/Warning.Warning'

	UPROPERTY(EditAnywhere, Category="UProperty - Difficulty Bar")
	USoundBase* SoundAlarmBell; //Script/Engine.SoundWave'/Game/Sounds/Warning/AlarmBell.AlarmBell'


	/*UPROPERTY(VisibleAnywhere, Category="UProperty - Difficulty Bar")
    	USGObjectiveToolTipWidget* ObjectiveToolTipWidget;*/
	/*UFUNCTION()
	void AnimationTriggerAlert(int NewDifficultLevel);
	float GetDifficultBarScrollSpeed() const;
	TArray<UOverlay*> GetOverlays();
	void MoveOverlaysLeft(float TranslationMovement);*/
};
