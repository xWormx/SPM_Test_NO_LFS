// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/SGEndGameInteractWidget.h"

#include "Components/SizeBox.h"

void USGEndGameInteractWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AddToViewport(10);
	SizeBoxEscape->SetVisibility(ESlateVisibility::Hidden);
}

void USGEndGameInteractWidget::ShowEscapeWidget()
{
	SizeBoxEscape->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void USGEndGameInteractWidget::HideEscapeWidget()
{
	SizeBoxEscape->SetVisibility(ESlateVisibility::Hidden);
}
