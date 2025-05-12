// Fill out your copyright notice in the Description page of Project Settings.


#include "Objectives/SGDifficultyBarWidget.h"

#include "Components/Overlay.h"

void USGDifficultyBarWidget::MoveOverlaysLeft(float TranslationMovement)
{
	for (UOverlay* overlay : Overlays)
	{
		overlay->SetRenderTranslation(FVector2D(400 - TranslationMovement, 0.0f));
	}
}

void USGDifficultyBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Overlays.Add(OverlayDifficulty);
	Overlays.Add(OverlayDifficulty_1);
	Overlays.Add(OverlayDifficulty_2);
	Overlays.Add(OverlayDifficulty_3);
}
