// Fill out your copyright notice in the Description page of Project Settings.


#include "SGObjectiveToolTipWidget.h"

#include "Components/TextBlock.h"

void USGObjectiveToolTipWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void USGObjectiveToolTipWidget::SetToolTipText(FText NewToolTip)
{
	ToolTip->SetText(NewToolTip);
}
