// Fill out your copyright notice in the Description page of Project Settings.


#include "Objectives/SGObjectiveBase.h"
#include "Objectives/SGGameObjectivesHandler.h"
#include "Objectives/SGObjectiveToolTipWidget.h"
// Sets default values
ASGObjectiveBase::ASGObjectiveBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ASGObjectiveBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASGObjectiveBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASGObjectiveBase::ActivateObjective()
{
	bIsActivated = true;
}

void ASGObjectiveBase::OnStart(ASGGameObjectivesHandler* ObjectivesHandler)
{
	if (!ObjectiveSubToolTips.IsEmpty())
		CurrentSubToolTip = ObjectiveSubToolTips[0];
	DisplayStartToolTip(ObjectivesHandler->GetObjectiveToolTipWidget());
}

void ASGObjectiveBase::OnCompleted(ASGGameObjectivesHandler* ObjectiveHandler)
{
	DisplayEndToolTip(ObjectiveHandler->GetObjectiveToolTipWidget());
}

void ASGObjectiveBase::DisplayStartToolTip(USGObjectiveToolTipWidget* ToolTipWidget)
{
	ToolTipWidget->Display(GetObjectiveDescriptionToolTip());
}

void ASGObjectiveBase::DisplayEndToolTip(USGObjectiveToolTipWidget* ToolTipWidget)
{
	ToolTipWidget->Display(GetObjectiveCompletedToolTip());
}

void ASGObjectiveBase::DisplayToolTip(USGObjectiveToolTipWidget* ToolTipWidget, const FText& ToolTip)
{
	ToolTipWidget->Display(ToolTip);
}

FText ASGObjectiveBase::GetCurrentSubToolTip()
{
	return FText::FromString(CurrentSubToolTip);
}

FText ASGObjectiveBase::GetObjectiveDescriptionToolTip()
{
	return FText::FromString(ObjectiveDescriptionToolTip);
}

FText ASGObjectiveBase::GetObjectiveCompletedToolTip()
{
	return FText::FromString(ObjectiveCompletedToolTip);
}




