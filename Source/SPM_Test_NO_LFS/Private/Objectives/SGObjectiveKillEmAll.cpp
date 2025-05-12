// Fill out your copyright notice in the Description page of Project Settings.


#include "Objectives/SGObjectiveKillEmAll.h"

#include "Objectives/SGGameObjectivesHandler.h"
#include "Objectives/SGHorizontalBoxObjective.h"

void ASGObjectiveKillEmAll::OnStart(ASGGameObjectivesHandler* ObjectiveHandler)
{
	Super::OnStart(ObjectiveHandler);
	ObjectiveHandler->GetObjectiveToolTipWidget()->AddProgressTextElement(FText::FromString(TEXT("ENEMIES KILLED:")), FText::FromString(TEXT("0/0")));
}

bool ASGObjectiveKillEmAll::IsCompleted(ASGGameObjectivesHandler* ObjectiveHandler)
{
	return CurrentEnemiesKilled == EnemiesToKill;
}

void ASGObjectiveKillEmAll::OnCompleted(ASGGameObjectivesHandler* ObjectiveHandler)
{
	Super::OnCompleted(ObjectiveHandler);
	USGHorizontalBoxObjective* KillEmAllProgressText = ObjectiveHandler->GetObjectiveToolTipWidget()->GetHorizontalBoxAtIndex(0);
	KillEmAllProgressText->ShowSucceed();
	KillEmAllProgressText->SetKeyAndValueOpacity(0.5);
	KillEmAllProgressText->SetValue(FText::FromString("Completed!"));
}

void ASGObjectiveKillEmAll::Update(ASGGameObjectivesHandler* ObjectiveHandler)
{
	CurrentEnemiesKilled++;
	FString StrEnemiesKilled = FString::Printf(TEXT("%d/%d"), CurrentEnemiesKilled, EnemiesToKill);
	//SetCurrentProgressText(StrEnemiesKilled);
	ObjectiveHandler->GetObjectiveToolTipWidget()->GetHorizontalBoxAtIndex(0)->SetValue(FText::FromString(StrEnemiesKilled));
}
