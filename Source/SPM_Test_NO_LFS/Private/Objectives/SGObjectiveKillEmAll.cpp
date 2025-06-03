// Fill out your copyright notice in the Description page of Project Settings.


#include "Objectives/SGObjectiveKillEmAll.h"
#include "Objectives/SGObjectiveToolTipWidget.h"
#include "Core/SGObjectiveHandlerSubSystem.h"
#include "Objectives/SGGameObjectivesHandler.h"
#include "Objectives/SGHorizontalBoxObjective.h"

void ASGObjectiveKillEmAll::OnStart()
{
	Super::OnStart();
	FString StrEnemiesKilled = FString::Printf(TEXT("%d/%d"), CurrentEnemiesKilled, EnemiesToKill);

	ObjectiveHandlerSubSystem->GetObjectiveToolTipWidget()->CreateProgressTextElement(
		GetObjectiveID(),
		FText::FromString(ObjectiveProgressText[0]),
		FText::FromString(StrEnemiesKilled));
}

bool ASGObjectiveKillEmAll::IsCompleted()
{
	return CurrentEnemiesKilled == EnemiesToKill;
}

void ASGObjectiveKillEmAll::OnCompleted()
{
	Super::OnCompleted();

	SetCurrentProgressElementCompleted("Completed!");
	
	USGHorizontalBoxObjective* HBoxObjective = ObjectiveHandlerSubSystem->GetObjectiveToolTipWidget()->GetHorizontalBoxObjective(this, 0);
	HBoxObjective->PlayAnimationValueCompleted();
}

void ASGObjectiveKillEmAll::Update()
{
	CurrentEnemiesKilled++;
	FString StrEnemiesKilled = FString::Printf(TEXT("%d/%d"), CurrentEnemiesKilled, EnemiesToKill);

	USGHorizontalBoxObjective* HBoxObjective = ObjectiveHandlerSubSystem->GetObjectiveToolTipWidget()->GetHorizontalBoxObjective(this, 0);
	HBoxObjective->SetValue(FText::FromString(StrEnemiesKilled));
}
