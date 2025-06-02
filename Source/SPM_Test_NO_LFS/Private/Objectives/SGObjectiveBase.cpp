// Fill out your copyright notice in the Description page of Project Settings.


#include "Objectives/SGObjectiveBase.h"

#include "SPM_Test_NO_LFS.h"
#include "Core/SGObjectiveHandlerSubSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Objectives/SGGameObjectivesHandler.h"
#include "Objectives/SGHorizontalBoxObjective.h"
#include "Objectives/SGObjectiveToolTipWidget.h"
#include "Objectives/SaveData/SGObjectiveSaveData.h"
// Sets default values
ASGObjectiveBase::ASGObjectiveBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void ASGObjectiveBase::BeginPlay()
{
	Super::BeginPlay();
	ObjectiveHandlerSubSystem = GetWorld()->GetSubsystem<USGObjectiveHandlerSubSystem>();
	if (ObjectiveHandlerSubSystem == nullptr)
	{
		CALLE_LOG(Error, TEXT("ObjectiveBase: Couldn't find ObjectiveHandlerSubSystem"));
	}
}

void ASGObjectiveBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASGObjectiveBase::LoadCompleted()
{
	for (int i = 0; i < ObjectiveProgressText.Num(); i++)
	{
		HorizontalBoxProgressElement.Add(ObjectiveHandlerSubSystem->GetObjectiveToolTipWidget()->CreateProgressTextElementCompleted(
			FText::FromString(ObjectiveProgressText[i]),
			FText::FromString("Completed")));
	}
	
}

void ASGObjectiveBase::OnStart()
{
	if (!ObjectiveSubToolTips.IsEmpty())
		CurrentSubToolTip = ObjectiveSubToolTips[0];

	DisplayStartToolTip(ObjectiveHandlerSubSystem->GetObjectiveToolTipWidget());

	CurrentSubObjectiveStep = 0;

	OnObjectiveStart.Broadcast(this);
}

void ASGObjectiveBase::OnCompleted()
{
	DisplayEndToolTip(ObjectiveHandlerSubSystem->GetObjectiveToolTipWidget());
}

void ASGObjectiveBase::Update()
{
	
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

USGObjectiveSaveData* ASGObjectiveBase::Save()
{
	USGObjectiveSaveData* SaveData = NewObject<USGObjectiveSaveData>();

	SaveBaseData(SaveData);

	return SaveData;
}

void ASGObjectiveBase::SaveBaseData(USGObjectiveSaveData* SaveData)
{
	SaveData->ObjectiveProgressText = ObjectiveProgressText;
	SaveData->CurrentSubObjectiveStep = CurrentSubObjectiveStep;
	SaveData->ObjectiveDescriptionToolTip = ObjectiveDescriptionToolTip;
	SaveData->ObjectiveCompletedToolTip = ObjectiveCompletedToolTip;
	SaveData->CurrentSubToolTip = CurrentSubToolTip;
	SaveData->ObjectiveSubToolTips = ObjectiveSubToolTips;
}


void ASGObjectiveBase::SetCurrentProgressElementCompleted(FString InTextCompleted)
{
	int ProgressStep = GetCurrentProgressStep();
	HorizontalBoxProgressElement[ProgressStep]->ShowSucceed();
	HorizontalBoxProgressElement[ProgressStep]->SetValue(FText::FromString(InTextCompleted));
	HorizontalBoxProgressElement[ProgressStep]->SetKeyAndValueOpacity(0.5);
}

FText ASGObjectiveBase::GetObjectiveDescriptionToolTip()
{
	return FText::FromString(ObjectiveDescriptionToolTip);
}

FText ASGObjectiveBase::GetObjectiveCompletedToolTip()
{
	return FText::FromString(ObjectiveCompletedToolTip);
}




