// Fill out your copyright notice in the Description page of Project Settings.


#include "Objectives/SGObjectiveCollectEmAll.h"

#include "Objectives/SGGameObjectivesHandler.h"
#include "Objectives/SGHorizontalBoxObjective.h"

void ASGObjectiveCollectEmAll::OnStart(ASGGameObjectivesHandler* ObjectiveHandler)
{
	Super::OnStart(ObjectiveHandler);
	FString StrCollected = FString::Printf(TEXT("%d/%d"), CurrentCollected, CollectGoal);
	ObjectiveHandler->GetObjectiveToolTipWidget()->AddProgressTextElement(FText::FromString("Fueldrods:"), FText::FromString(StrCollected));
}

bool ASGObjectiveCollectEmAll::IsCompleted(ASGGameObjectivesHandler* ObjectiveHandler)
{
	return CurrentCollected == CollectGoal;	
}

void ASGObjectiveCollectEmAll::OnCompleted(ASGGameObjectivesHandler* ObjectiveHandler)
{
	Super::OnCompleted(ObjectiveHandler);

	USGHorizontalBoxObjective* CollectProgressText = ObjectiveHandler->GetObjectiveToolTipWidget()->GetHorizontalBoxAtIndex(1);
	CollectProgressText->ShowSucceed();
	CollectProgressText->SetKeyAndValueOpacity(0.5);
	CollectProgressText->SetValue(FText::FromString("Completed!"));
	
}

void ASGObjectiveCollectEmAll::Update(ASGGameObjectivesHandler* ObjectiveHandler)
{
	CurrentCollected++;
	FString StrCollected = FString::Printf(TEXT("%d/%d"), CurrentCollected, CollectGoal);
	//SetCurrentProgressText(StrCollected);
	
	if (USGHorizontalBoxObjective* CollectProgressText = ObjectiveHandler->GetObjectiveToolTipWidget()->GetHorizontalBoxAtIndex(1))
		CollectProgressText->SetValue(FText::FromString(StrCollected));
}
