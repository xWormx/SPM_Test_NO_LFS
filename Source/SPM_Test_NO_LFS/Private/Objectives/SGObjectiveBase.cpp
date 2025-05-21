// Fill out your copyright notice in the Description page of Project Settings.


#include "Objectives/SGObjectiveBase.h"

#include "SPM_Test_NO_LFS.h"
#include "Core/SGObjectiveHandlerSubSystem.h"
#include "Kismet/GameplayStatics.h"
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
	ObjectiveHandlerSubSystem = GetWorld()->GetSubsystem<USGObjectiveHandlerSubSystem>();
	if (ObjectiveHandlerSubSystem == nullptr)
	{
		CALLE_LOG(Error, TEXT("ObjectiveBase: Couldn't find ObjectiveHandlerSubSystem"));
	}
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
/*
void ASGObjectiveBase::OnStart(ASGGameObjectivesHandler* ObjectivesHandler)
{
	if (ObjectivesHandler == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ASGObjectiveBase::OnStart - ObjectivesHandler was nullptr!"));
		return;
	}
	
	//SetObjectiveHandler(ObjectivesHandler);
	if (!ObjectiveSubToolTips.IsEmpty())
		CurrentSubToolTip = ObjectiveSubToolTips[0];
	//DisplayStartToolTip(ObjectivesHandler->GetObjectiveToolTipWidget());
	DisplayStartToolTip(ObjectiveHandlerSubSystem->GetObjectiveToolTipWidget());
	CurrentSubObjectiveStep = 1;

	OnObjectiveStart.Broadcast(this);
}

void ASGObjectiveBase::OnCompleted(ASGGameObjectivesHandler* ObjectiveHandler)
{
	//DisplayEndToolTip(ObjectiveHandler->GetObjectiveToolTipWidget());
	DisplayEndToolTip(ObjectiveHandlerSubSystem->GetObjectiveToolTipWidget());
	
}
*/
void ASGObjectiveBase::OnStart()
{
	if (!ObjectiveSubToolTips.IsEmpty())
		CurrentSubToolTip = ObjectiveSubToolTips[0];
	//DisplayStartToolTip(ObjectivesHandler->GetObjectiveToolTipWidget());
	DisplayStartToolTip(ObjectiveHandlerSubSystem->GetObjectiveToolTipWidget());
	CurrentSubObjectiveStep = 1;

	OnObjectiveStart.Broadcast(this);
}

void ASGObjectiveBase::OnCompleted()
{
	DisplayEndToolTip(ObjectiveHandlerSubSystem->GetObjectiveToolTipWidget());
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

void ASGObjectiveBase::SetCurrentProgressText(FString NewCurrentProgressText)
{
	if (ProgressText.SubText.IsEmpty())
	{
		//UE_LOG(LogTemp, Error, TEXT("Progress SubText (Whole array) was empty in Objective: %s"), *GetActorLabel())
		return;

	}
	ProgressText.SubText[GetCurrentProgressStep() - 1] = NewCurrentProgressText;
	//GetObjectiveHandler()->GetObjectiveToolTipWidget()->SetProgressWindowText(this);
}

FText ASGObjectiveBase::GetObjectiveDescriptionToolTip()
{
	return FText::FromString(ObjectiveDescriptionToolTip);
}

FText ASGObjectiveBase::GetObjectiveCompletedToolTip()
{
	return FText::FromString(ObjectiveCompletedToolTip);
}




