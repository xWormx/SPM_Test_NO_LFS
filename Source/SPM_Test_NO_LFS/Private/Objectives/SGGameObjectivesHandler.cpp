// Fill out your copyright notice in the Description page of Project Settings.


#include "Objectives/SGGameObjectivesHandler.h"
#include "Enemies/Characters/SGEnemyCharacter.h"
#include "Objectives/SGObjectiveBase.h"
#include "Objectives/SGObjectiveToolTipWidget.h"
#include "Pickups/SGPickUpObjectiveCollect.h"
#include "Player/SGPlayerCharacter.h"
#include "Player/SGPlayerController.h"
#include "Objectives/SGTerminalWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Objectives/SGHorizontalBoxObjective.h"
#include "Objectives/SGObjectiveDefendThePod.h"

// Sets default values
ASGGameObjectivesHandler::ASGGameObjectivesHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASGGameObjectivesHandler::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("ASGGameObjectivesHandler::BeginPlay, there is a objectivehandler"));

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		ObjectiveToolTipWidget = Cast<USGObjectiveToolTipWidget>(CreateWidget<USGObjectiveToolTipWidget>(PlayerController, ObjectiveToolTipClass));
		ObjectiveToolTipWidget->AddToViewport(5); // Should be lower than TerminalWidget!
		ObjectiveToolTipWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
		ObjectiveToolTipWidget->ShowVisitTerminal();
		ObjectiveToolTipWidget->HideToolTipScaleBox();
		ObjectiveToolTipWidget->SetFadeFactor(ObjectiveToolTipFadeFactor);
	}
}

void ASGGameObjectivesHandler::RegisterEnemy(ASGEnemyCharacter* Enemy)
{
	if (Enemy == nullptr)
		return;

	Enemy->OnEnemyDiedObjective.AddDynamic(this, &ASGGameObjectivesHandler::UpdateCurrentGameObjective);
	TargetCharacters.Push(Enemy);
}

void ASGGameObjectivesHandler::RegisterCollectible(ASGPickUpObjectiveCollect* Collectible)
{
	if (Collectible == nullptr)
		return;

	if (!Collectible->OnCollected.IsAlreadyBound(this, &ASGGameObjectivesHandler::UpdateCurrentGameObjective))
	{
		Collectible->OnCollected.AddDynamic(this, &ASGGameObjectivesHandler::UpdateCurrentGameObjective);
		TargetCollectibles.Push(Collectible);		
	}

}

void ASGGameObjectivesHandler::RegisterTerminalWidget(USGTerminalWidget* TerminalWidget)
{
	
	if (TerminalWidget == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("TerminalWidget was Valid."));
		return;
	}
		
	TerminalHUD = TerminalWidget;

	if (TerminalHUD == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ObjectiveHandlers TerminalHUD was NULL!"));
		return;
	}

	if (!TerminalHUD->OnStartMission.IsAlreadyBound(this, &ASGGameObjectivesHandler::StartMission))
		TerminalHUD->OnStartMission.AddDynamic(this, &ASGGameObjectivesHandler::StartMission);
}

void ASGGameObjectivesHandler::RegisterDefendThePod(ASGObjectiveDefendThePod* DefendThePod)
{
	if (DefendThePod == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ObjectiveHandler: DefendThePod was nullptr!"));
		return;
	}

	DefendThePod->OnDefendEventEnd.AddDynamic(this, &ASGGameObjectivesHandler::UpdateCurrentGameObjective);
		
}

void ASGGameObjectivesHandler::StartMission()
{
	/*
		Tilldela varje objective ett ID här så att det kan lagras i ToolTipWidget's TMap så att
		progresswindow kan hålla koll på alla texter 
	 
	 */
	ObjectiveToolTipWidget->HideVisitTerminal();
	if (GameObjectives.Num() > 0)
		CurrentObjective = GameObjectives[0];
	
	if (CurrentObjective == nullptr)
	{
		FString str = FString::Printf(TEXT("No more objectives!"));
		ObjectiveToolTipWidget->Display(FText::FromString(str));
		return;
	}
	
	CurrentObjective->OnStart(this);
	TerminalHUD->DisableStartButton();
	UGameplayStatics::PlaySound2D(this, MissionStartedSound);
	OnObjectiveStarted.Broadcast();
}

// TODO: Ändra parameter till TSubscriptInterface<ISGObjectiveInterface> eller vad den nu hette...
void ASGGameObjectivesHandler::UpdateCurrentGameObjective(UObject* ObjectiveInterfaceImplementor)
{
	EObjectiveType IncomingObjectiveType = EObjectiveType::EOT_InvalidObjectiveType;
	ISGObjectiveInterface* Objective = Cast<ISGObjectiveInterface>(ObjectiveInterfaceImplementor);
	if (Objective)
	{
		IncomingObjectiveType = Objective->GetObjectiveType();
		UE_LOG(LogTemp, Warning, TEXT("Incoming ObjectiveType: %d"), IncomingObjectiveType);
	}

	if (CurrentObjective == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("CurrentObjective is nullptr!"));
		return;
	}
	// Om fel objectivetype har broadcastat så behöver vi inte uppdatera CurrenObjective.
	if (CurrentObjective->GetObjectiveType() != IncomingObjectiveType)
		return;
	
	CurrentObjective->Update(this);
	ObjectiveToolTipWidget->GetCurrentHorizontalBoxObjective()->PlayUpdateValueAnimation();
	if (CurrentObjective->IsCompleted(this))
	{
		ObjectiveToolTipWidget->ShowVisitTerminal();
		LastCompletedObjective = GetCurrentObjective();
		CurrentObjective->OnCompleted(this);
		UGameplayStatics::PlaySound2D(this, MissionCompletedSound);
		RemoveCurrentObjective();
		OnObjectiveCompleted.Broadcast();
	}
}

void ASGGameObjectivesHandler::RemoveCurrentObjective()
{
	// Ta bort avklarat objective från Arrayen
	if (GameObjectives.Num() > 0)
	{
		GameObjectives.RemoveAt(0);
		if (GameObjectives.Num() <= 0)
		{
			TerminalHUD->DisableStartButton();
		}
		else
		{
			TerminalHUD->EnableStartButton();
		}
		
	}
	// Borde CurrentObjective vara något annat istället för nullptr när det inte är aktiverat?
	CurrentObjective = nullptr;		
}


