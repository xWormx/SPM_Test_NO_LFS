// Fill out your copyright notice in the Description page of Project Settings.


#include "SGTerminalWidget.h"

#include "Components/Button.h"

void USGTerminalWidget::NativeConstruct()
{
	if (ButtonStartMission)
	{
		ButtonStartMission->OnClicked.AddDynamic(this, &USGTerminalWidget::OnClickStartMission);
		ButtonStartMission->OnHovered.AddDynamic(this, &USGTerminalWidget::OnHoverStartMission);
	}
}

void USGTerminalWidget::OnClickStartMission()
{
	ButtonStartMission->SetBackgroundColor(FLinearColor::Red);
}

void USGTerminalWidget::OnHoverStartMission()
{
	
}

