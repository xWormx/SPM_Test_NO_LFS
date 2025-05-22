// Fill out your copyright notice in the Description page of Project Settings.


#include "Objectives/SGObjectiveFinalSweep.h"

#include "Core/SGObjectiveHandlerSubSystem.h"
#include "Objectives/SGHorizontalBoxObjective.h"

bool ASGObjectiveFinalSweep::IsCompleted()
{
	return Super::IsCompleted();
}

void ASGObjectiveFinalSweep::OnStart()
{
	Super::OnStart();
	HorizontalBoxProgressElement.Add(ObjectiveHandlerSubSystem->GetObjectiveToolTipWidget()->CreateProgressTextElement(FText::FromString(ObjectiveProgressText[0]), FText::FromString("0")));
}

void ASGObjectiveFinalSweep::OnCompleted()
{
	Super::OnCompleted();
}

void ASGObjectiveFinalSweep::Update()
{
	Super::Update();
	IncrementEnemiesKilled();
	FString StrEnemiesKilled = FString::Printf(TEXT("%d"), CurrentEnemiesKilled);
	HorizontalBoxProgressElement[0]->SetValue(FText::FromString(StrEnemiesKilled));
}

void ASGObjectiveFinalSweep::IncrementEnemiesKilled()
{
	CurrentEnemiesKilled++;
}
