// Fill out your copyright notice in the Description page of Project Settings.


#include "Objectives/SGTerminalWidget.h"
#include "Components/Button.h"
#include "Core/SGObjectiveHandlerSubSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Objectives/SGGameObjectivesHandler.h"

void USGTerminalWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ObjectiveHandlerSubSystem = GetWorld()->GetSubsystem<USGObjectiveHandlerSubSystem>();
	if (ButtonStartMission)
	{
		ButtonStartMission->OnClicked.AddDynamic(this, &USGTerminalWidget::OnClickStartMission);
		ButtonStartMission->OnHovered.AddDynamic(this, &USGTerminalWidget::OnHoverStartMission);
		ButtonStartMission->OnUnhovered.AddDynamic(this, &USGTerminalWidget::OnUnHoverStartMission);
		ButtonStartMission->OnPressed.AddDynamic(this, &USGTerminalWidget::OnPressStartMission);
		ButtonStartMission->OnReleased.AddDynamic(this, &USGTerminalWidget::OnReleaseStartMission);
		//GameObjectivesHandler->RegisterTerminalWidget(this);
	}

	if (ButtonCloseTerminal)
	{
		ButtonCloseTerminal->OnClicked.AddDynamic(this, &USGTerminalWidget::OnClickCloseTerminal);
		ButtonCloseTerminal->OnHovered.AddDynamic(this, &USGTerminalWidget::OnHoverCloseTerminal);
		ButtonCloseTerminal->OnUnhovered.AddDynamic(this, &USGTerminalWidget::OnUnHoverCloseTerminal);
		ButtonCloseTerminal->OnPressed.AddDynamic(this, &USGTerminalWidget::OnPressCloseMission);
		ButtonCloseTerminal->OnReleased.AddDynamic(this, &USGTerminalWidget::OnReleaseCloseMission);
	}
}
/*
void USGTerminalWidget::SetObjectiveHandler(ASGGameObjectivesHandler* ObjectiveHandler)
{
	//GameObjectivesHandler = ObjectiveHandler;
	
}
*/
void USGTerminalWidget::OnPressStartMission()
{
	UGameplayStatics::PlaySound2D(this, SoundPressButton);
}

void USGTerminalWidget::OnReleaseStartMission()
{
	UGameplayStatics::PlaySound2D(this, SoundReleaseButton);
}

void USGTerminalWidget::OnClickStartMission()
{
	//GameObjectivesHandler->RegisterTerminalWidget(this);
	ObjectiveHandlerSubSystem->RegisterTerminalWidget(this);
	//ButtonStartMission->SetBackgroundColor(FLinearColor::Red);
	OnStartMission.Broadcast();
}

void USGTerminalWidget::OnHoverStartMission()
{
	//ButtonStartMission->SetBackgroundColor(FLinearColor::Green);
	UE_LOG(LogTemp, Warning, TEXT("Play hover sound"));
	UGameplayStatics::PlaySound2D(GetWorld(), SoundHoverButton);
	
}

void USGTerminalWidget::OnUnHoverStartMission()
{
	//ButtonStartMission->SetBackgroundColor(FLinearColor::Blue);
}

void USGTerminalWidget::OnPressCloseMission()
{
	UGameplayStatics::PlaySound2D(this, SoundPressButton);
}

void USGTerminalWidget::OnReleaseCloseMission()
{
	UGameplayStatics::PlaySound2D(this, SoundReleaseButton);
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
	UGameplayStatics::PlaySound2D(GetWorld(), SoundHoverButton);
}

void USGTerminalWidget::OnUnHoverCloseTerminal()
{
}
