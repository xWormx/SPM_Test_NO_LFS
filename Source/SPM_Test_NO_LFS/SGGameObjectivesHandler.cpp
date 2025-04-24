// Fill out your copyright notice in the Description page of Project Settings.


#include "SGGameObjectivesHandler.h"
#include "SGEnemyCharacter.h"
#include "SGObjectiveBase.h"
#include "SGObjectiveToolTipWidget.h"
#include "SGPlayerCharacter.h"
#include "SGPlayerController.h"
#include "SGTerminalWidget.h"
#include "Components/Button.h"

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
		ObjectiveToolTipWidget->AddToViewport();
		ObjectiveToolTipWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ASGGameObjectivesHandler::RegisterEnemy(ASGEnemyCharacter* Enemy)
{
	if (Enemy == nullptr)
		return;

	Enemy->OnEnemyDied.AddDynamic(this, &ASGGameObjectivesHandler::UpdateCurrentGameObjective);
	TargetCharacters.Push(Enemy);
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

void ASGGameObjectivesHandler::StartMission()
{
	if (ObjectiveCounter < GameObjectives.Num())
		CurrentObjective = GameObjectives[ObjectiveCounter++];
	
	if (CurrentObjective == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("CurrentObjective is nullptr!"))
		return;
	}
	FString str = FString::Printf(TEXT("StartMission: %s"), *CurrentObjective->GetName());
	UE_LOG(LogTemp, Warning, TEXT("StartMission: %s"), *str);
	ObjectiveToolTipWidget->Display(CurrentObjective->GetToolTipText());
	/*
	 StartNextObjectiveInPipeline();
	*/
}

void ASGGameObjectivesHandler::UpdateCurrentGameObjective(ASGEnemyCharacter* Actor)
{
	if (CurrentObjective == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("CurrentObjective is nullptr!"));
		return;
	}
	
	CurrentObjective->Update();
	if (CurrentObjective->CheckProgress())
	{
		UE_LOG(LogTemp, Warning, TEXT("CurrentObjective Done!"));
		FString str = FString::Printf(TEXT("Mission Accomplished: %s"), *CurrentObjective->GetName());
		ObjectiveToolTipWidget->Display(FText::FromString(str));

		// Om det finns några objectives, ta bort den första i listan (som blev avklarad)
		// Finns det kvar några obectives så sätt Current till nästa i listan (som nu på på [0]).
		if (GameObjectives.Num() > 0)
		{
			GameObjectives.RemoveAt(0);
			if (GameObjectives.Num() > 0)
			{
				CurrentObjective = GameObjectives[0];
			}
		}
	}
}


void ASGGameObjectivesHandler::StartNextObjective(EObjectiveType NextObjectiveType)
{
	//CurrentObjective = NextObjectiveType;
}


