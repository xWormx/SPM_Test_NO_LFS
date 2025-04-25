// Fill out your copyright notice in the Description page of Project Settings.


#include "SGGameObjectivesHandler.h"
#include "SGEnemyCharacter.h"
#include "SGObjectiveBase.h"
#include "SGObjectiveToolTipWidget.h"
#include "SGPickUpObjectiveCollect.h"
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

void ASGGameObjectivesHandler::StartMission()
{
	if (ObjectiveCounter < GameObjectives.Num())
		CurrentObjective = GameObjectives[ObjectiveCounter++];
	
	if (CurrentObjective == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("CurrentObjective is nullptr!"))
		FString str = FString::Printf(TEXT("No more objectives!"));
		UE_LOG(LogTemp, Warning, TEXT("StartMission: %s"), *str);
		ObjectiveToolTipWidget->Display(FText::FromString(str));
		return;
	}
	FString str = FString::Printf(TEXT("StartMission: %s"), *CurrentObjective->GetName());
	UE_LOG(LogTemp, Warning, TEXT("StartMission: %s"), *str);
	ObjectiveToolTipWidget->Display(CurrentObjective->GetToolTipText());
	/*
	 StartNextObjectiveInPipeline();
	*/
}

void ASGGameObjectivesHandler::UpdateCurrentGameObjective(UObject* ObjectiveInterfaceImplementor)
{
	EObjectiveType IncomingObjectiveType = EObjectiveType::EOT_KillAllEnemies;
	if (ASGEnemyCharacter* enemy = Cast<ASGEnemyCharacter>(ObjectiveInterfaceImplementor))
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyKilled"));
		IncomingObjectiveType = enemy->GetObjectiveType();
	}
	else if (ASGPickUpObjectiveCollect* collectible = Cast<ASGPickUpObjectiveCollect>(ObjectiveInterfaceImplementor))
	{
		UE_LOG(LogTemp, Warning, TEXT("Collectible"));
		IncomingObjectiveType = collectible->GetObjectiveType();
	}

	if (CurrentObjective == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("CurrentObjective is nullptr!"));
		return;
	}
	// Om fel objectivetype har broadcastat så behöver vi inte uppdatera CurrenObjective.
	if (CurrentObjective->GetObjectiveType() != IncomingObjectiveType)
		return;
	
	CurrentObjective->Update();
	if (CurrentObjective->CheckProgress())
	{
		UE_LOG(LogTemp, Warning, TEXT("CurrentObjective Done!"));
		FString str = FString::Printf(TEXT("Mission Accomplished: %s"), *CurrentObjective->GetName());
		ObjectiveToolTipWidget->Display(FText::FromString(str));

		// Om det finns några objectives, ta bort den första i listan (som blev avklarad)
		// Finns det kvar några obectives så sätt Current till nästa i listan (som nu är på [0]).
		if (GameObjectives.Num() > 0)
		{
			GameObjectives.RemoveAt(0);
			if (GameObjectives.Num() > 0)
			{
				CurrentObjective = GameObjectives[0];
			}
			else
			{
				CurrentObjective = nullptr;
			}
		}
	}
}



