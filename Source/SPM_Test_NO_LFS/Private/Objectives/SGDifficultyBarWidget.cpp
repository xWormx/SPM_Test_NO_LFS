// Fill out your copyright notice in the Description page of Project Settings.


#include "Objectives/SGDifficultyBarWidget.h"

#include "Components/Overlay.h"
#include "Core/SGGameInstance.h"

void USGDifficultyBarWidget::MoveOverlaysLeft(float TranslationMovement)
{
	for (UOverlay* overlay : Overlays)
	{
		overlay->SetRenderTranslation(FVector2D(DifficultBoxStartPosition - TranslationMovement, 0.0f));
	}
}	

void USGDifficultyBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
	USGGameInstance* GameInstance = Cast<USGGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		ObjectiveToolTipWidget = GameInstance->GetObjectiveTooltipWidget();
		if (ObjectiveToolTipWidget)
		{
			ObjectiveToolTipWidget->OnDifficultyChanged.AddDynamic(this, &USGDifficultyBarWidget::AnimationTriggerAlert);	
		}
	}

	OverlayWarningMessage->SetVisibility(ESlateVisibility::Hidden);
	Overlays.Add(OverlayDifficulty);
	Overlays.Add(OverlayDifficulty_1);
	Overlays.Add(OverlayDifficulty_2);
	Overlays.Add(OverlayDifficulty_3);
}

void USGDifficultyBarWidget::AnimationTriggerAlert(int NewDifficultLevel)
{
	PlayAnimation(AnimationShowWarningMessage);
	PlayAnimation(AnimationTriggerNewDifficultLevel);
}

