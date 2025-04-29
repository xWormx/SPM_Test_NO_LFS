// Fill out your copyright notice in the Description page of Project Settings.


#include "Objectives/SGTerminalWidget.h"
#include "Components/Button.h"
#include "Objectives/SGGameObjectivesHandler.h"

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

	if (ButtonCloseTerminal)
	{
		ButtonCloseTerminal->OnClicked.AddDynamic(this, &USGTerminalWidget::OnClickCloseTerminal);
		ButtonCloseTerminal->OnHovered.AddDynamic(this, &USGTerminalWidget::OnHoverCloseTerminal);
		ButtonCloseTerminal->OnUnhovered.AddDynamic(this, &USGTerminalWidget::OnUnHoverCloseTerminal);
	}
}

void USGTerminalWidget::SetObjectiveHandler(ASGGameObjectivesHandler* ObjectiveHandler)
{
	GameObjectivesHandler = ObjectiveHandler; 
}

void USGTerminalWidget::OnClickStartMission()
{
	GameObjectivesHandler->RegisterTerminalWidget(this);
	//ButtonStartMission->SetBackgroundColor(FLinearColor::Red);
	OnStartMission.Broadcast();
}

void USGTerminalWidget::OnHoverStartMission()
{
	//ButtonStartMission->SetBackgroundColor(FLinearColor::Green);
	
}

void USGTerminalWidget::OnUnHoverStartMission()
{
	//ButtonStartMission->SetBackgroundColor(FLinearColor::Blue);
}

void USGTerminalWidget::EnableStartButton()
{
	ButtonStartMission->SetIsEnabled(true);
}

void USGTerminalWidget::DisableStartButton()
{
	ButtonStartMission->SetIsEnabled(false);
}

void USGTerminalWidget::OnClickCloseTerminal()
{
	OnCloseTerminal.Broadcast();
}

void USGTerminalWidget::OnHoverCloseTerminal()
{
}

void USGTerminalWidget::OnUnHoverCloseTerminal()
{
}
