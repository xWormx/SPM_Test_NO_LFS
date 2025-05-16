// Fill out your copyright notice in the Description page of Project Settings.


#include "Objectives/SGObjectiveKillEmAll.h"

#include "Objectives/SGGameObjectivesHandler.h"
#include "Objectives/SGHorizontalBoxObjective.h"

void ASGObjectiveKillEmAll::OnStart(ASGGameObjectivesHandler* ObjectiveHandler)
{
	Super::OnStart(ObjectiveHandler);
	FString StrEnemiesKilled = FString::Printf(TEXT("%d/%d"), CurrentEnemiesKilled, EnemiesToKill);
	HorizontalBoxProgressElement.Add(ObjectiveHandler->GetObjectiveToolTipWidget()->CreateProgressTextElement(FText::FromString(TEXT("ENEMIES KILLED:")), FText::FromString(StrEnemiesKilled)));
}

bool ASGObjectiveKillEmAll::IsCompleted(ASGGameObjectivesHandler* ObjectiveHandler)
{
	return CurrentEnemiesKilled == EnemiesToKill;
}

void ASGObjectiveKillEmAll::OnCompleted(ASGGameObjectivesHandler* ObjectiveHandler)
{
	Super::OnCompleted(ObjectiveHandler);
	HorizontalBoxProgressElement[0]->ShowSucceed();
	HorizontalBoxProgressElement[0]->SetKeyAndValueOpacity(0.5);
	HorizontalBoxProgressElement[0]->SetValue(FText::FromString("Completed!"));
}

void ASGObjectiveKillEmAll::Update(ASGGameObjectivesHandler* ObjectiveHandler)
{
	CurrentEnemiesKilled++;
	FString StrEnemiesKilled = FString::Printf(TEXT("%d/%d"), CurrentEnemiesKilled, EnemiesToKill);
	HorizontalBoxProgressElement[0]->SetValue(FText::FromString(StrEnemiesKilled));
}
