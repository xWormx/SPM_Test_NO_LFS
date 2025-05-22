// Fill out your copyright notice in the Description page of Project Settings.


#include "Objectives/SGObjectiveKillEmAll.h"

#include "Core/SGObjectiveHandlerSubSystem.h"
#include "Objectives/SGGameObjectivesHandler.h"
#include "Objectives/SGHorizontalBoxObjective.h"

void ASGObjectiveKillEmAll::OnStart()
{
	Super::OnStart();
	FString StrEnemiesKilled = FString::Printf(TEXT("%d/%d"), CurrentEnemiesKilled, EnemiesToKill);
	HorizontalBoxProgressElement.Add(ObjectiveHandlerSubSystem->GetObjectiveToolTipWidget()->CreateProgressTextElement(FText::FromString(ObjectiveProgressText[0]), FText::FromString(StrEnemiesKilled)));
}

bool ASGObjectiveKillEmAll::IsCompleted()
{
	return CurrentEnemiesKilled == EnemiesToKill;
}

void ASGObjectiveKillEmAll::OnCompleted()
{
	Super::OnCompleted();
	HorizontalBoxProgressElement[0]->ShowSucceed();
	HorizontalBoxProgressElement[0]->SetKeyAndValueOpacity(0.5);
	HorizontalBoxProgressElement[0]->SetValue(FText::FromString("Completed!"));
}

void ASGObjectiveKillEmAll::Update()
{
	CurrentEnemiesKilled++;
	FString StrEnemiesKilled = FString::Printf(TEXT("%d/%d"), CurrentEnemiesKilled, EnemiesToKill);
	HorizontalBoxProgressElement[0]->SetValue(FText::FromString(StrEnemiesKilled));
}
