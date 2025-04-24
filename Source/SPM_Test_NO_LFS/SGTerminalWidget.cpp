// Fill out your copyright notice in the Description page of Project Settings.


#include "SGTerminalWidget.h"
#include "Components/Button.h"
#include "SGGameObjectivesHandler.h"

void USGTerminalWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (ButtonStartMission)
	{
		ButtonStartMission->OnClicked.AddDynamic(this, &USGTerminalWidget::OnClickStartMission);
		ButtonStartMission->OnHovered.AddDynamic(this, &USGTerminalWidget::OnHoverStartMission);
		ButtonStartMission->OnUnhovered.AddDynamic(this, &USGTerminalWidget::OnUnHoverStartMission);
		//GameObjectivesHandler->RegisterTerminalWidget(this);
	}
}

void USGTerminalWidget::SetObjectiveHandler(ASGGameObjectivesHandler* ObjectiveHandler)
{
	GameObjectivesHandler = ObjectiveHandler; 
}

void USGTerminalWidget::OnClickStartMission()
{
	GameObjectivesHandler->RegisterTerminalWidget(this);
	ButtonStartMission->SetBackgroundColor(FLinearColor::Red);
	OnStartMission.Broadcast();
	/*
	ObjectiveHandler->StartMission();
	ActivateMission();
	*/
}

void USGTerminalWidget::OnHoverStartMission()
{
	ButtonStartMission->SetBackgroundColor(FLinearColor::Blue);
}

void USGTerminalWidget::OnUnHoverStartMission()
{
	ButtonStartMission->SetBackgroundColor(FLinearColor::Gray);
}
