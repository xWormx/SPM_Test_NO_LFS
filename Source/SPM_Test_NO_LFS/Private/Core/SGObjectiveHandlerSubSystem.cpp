// Fill out your copyright notice in the Description page of Project Settings.
// comment

#include "Core/SGObjectiveHandlerSubSystem.h"
#include "Core/SGGameInstance.h"
#include "Enemies/Characters/SGEnemyCharacter.h"
#include "Objectives/SGObjectiveBase.h"
#include "Objectives/SGObjectiveToolTipWidget.h"
#include "Pickups/SGPickUpObjectiveCollect.h"
#include "Objectives/SGTerminalWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Objectives/SGDataAssetObjective.h"
#include "Objectives/SGHorizontalBoxObjective.h"
#include "Objectives/SGObjectiveConfig.h"
#include "Objectives/SGObjectiveDefendThePod.h"
#include "Objectives/SGObjectivePodArrival.h"


void USGObjectiveHandlerSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	CALLE_LOG(Warning, TEXT("ObjectiveHandler SubSystem Initialize"));
	
	FWorldDelegates::OnWorldInitializedActors.AddUObject(this, &USGObjectiveHandlerSubSystem::OnWorldInitialized);
}

void USGObjectiveHandlerSubSystem::Deinitialize()
{
	Super::Deinitialize();
}

void USGObjectiveHandlerSubSystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	USGGameInstance* GameInstance = Cast<USGGameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance)
	{
		// Måste skapa om dessa för att de ska synas, annars kommer OjbectiveTooltip vara kopplad till en Viewport i en annan level.
		GameInstance->CreateObjectiveToolTip();
		GameInstance->CreateHUDTerminal();
		ObjectiveToolTipWidget = GameInstance->GetObjectiveTooltipWidget();
		if (ObjectiveToolTipWidget)
		{
			
			ObjectiveToolTipWidget->SetOwningPlayer(GetWorld()->GetFirstPlayerController());
			
			if (!ObjectiveToolTipWidget->IsInViewport())
			{
				ObjectiveToolTipWidget->RemoveFromParent();
				ObjectiveToolTipWidget->AddToViewport(5); // Should be lower than TerminalWidget!
			}
				
			
			ObjectiveToolTipWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
			ObjectiveToolTipWidget->ShowVisitTerminal();
			ObjectiveToolTipWidget->HideToolTipScaleBox();	
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("ObjectiveHandler: Instance is of class %s, address: %p"), *GetClass()->GetName(), this);
	if (ObjectiveDataAsset == nullptr)
	{
		CALLE_LOG(Error, TEXT("ObjectiveDataAsset is null"));
	}
	else
	{
		for (USGObjectiveConfig* Config : ObjectiveDataAsset->ObjectiveConfigs)
		{
			if (!Config || !*Config->ObjectiveClass)
				continue;

			FActorSpawnParameters SpawnParams;
			ASGObjectiveBase* SpawnedObjective = GetWorld()->SpawnActor<ASGObjectiveBase>(Config->ObjectiveClass, SpawnParams);
			if (SpawnedObjective)
			{
				Config->ApplyData(SpawnedObjective);
				UE_LOG(LogTemp, Log, TEXT("Spawned and configured objective: %s"), *SpawnedObjective->GetName());
				GameObjectives.Add(SpawnedObjective);
			}
		}
	}
}

bool USGObjectiveHandlerSubSystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (this->GetClass()->IsInBlueprint() && Super::ShouldCreateSubsystem(Outer))
	{
		return true;
	}
	return false;
}

void USGObjectiveHandlerSubSystem::OnWorldInitialized(const UWorld::FActorsInitializedParams& Params)
{
	//ReadObjectives();
}

void USGObjectiveHandlerSubSystem::ReadObjectives()
{
	if (ObjectiveDataAsset == nullptr)
	{
		CALLE_LOG(Error, TEXT("ObjectiveDataAsset is null"));
	}
	else
	{
		for (USGObjectiveConfig* Config : ObjectiveDataAsset->ObjectiveConfigs)
		{
			if (!Config || !*Config->ObjectiveClass)
				continue;

			FActorSpawnParameters SpawnParams;
			ASGObjectiveBase* SpawnedObjective = GetWorld()->SpawnActor<ASGObjectiveBase>(Config->ObjectiveClass, SpawnParams);
			if (SpawnedObjective)
			{
				Config->ApplyData(SpawnedObjective);
				UE_LOG(LogTemp, Log, TEXT("Spawned and configured objective: %s"), *SpawnedObjective->GetName());
				GameObjectives.Add(SpawnedObjective);
			}
		}
	}
}
void USGObjectiveHandlerSubSystem::RegisterEnemy(ASGEnemyCharacter* Enemy)
{
	if (Enemy == nullptr)
		return;

	Enemy->OnEnemyDiedObjective.AddUniqueDynamic(this, &USGObjectiveHandlerSubSystem::UpdateCurrentGameObjective);
	TargetCharacters.Push(Enemy);
}

void USGObjectiveHandlerSubSystem::RegisterCollectible(ASGPickUpObjectiveCollect* Collectible)
{
	if (Collectible == nullptr)
		return;

	if (!Collectible->OnCollected.IsAlreadyBound(this, &USGObjectiveHandlerSubSystem::UpdateCurrentGameObjective))
	{
		Collectible->OnCollected.AddDynamic(this, &USGObjectiveHandlerSubSystem::UpdateCurrentGameObjective);
		TargetCollectibles.Push(Collectible);		
	}

}

void USGObjectiveHandlerSubSystem::RegisterTerminalWidget(USGTerminalWidget* TerminalWidget)
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

	if (!TerminalHUD->OnStartMission.IsAlreadyBound(this, &USGObjectiveHandlerSubSystem::StartMission))
		TerminalHUD->OnStartMission.AddDynamic(this, &USGObjectiveHandlerSubSystem::StartMission);
}

void USGObjectiveHandlerSubSystem::RegisterDefendThePod(ASGObjectiveDefendThePod* DefendThePod)
{
	if (DefendThePod == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ObjectiveHandler: DefendThePod was nullptr!"));
		return;
	}

	DefendThePod->OnDefendEventEnd.AddDynamic(this, &USGObjectiveHandlerSubSystem::UpdateCurrentGameObjective);
}

void USGObjectiveHandlerSubSystem::RegisterPodArrival(ASGObjectivePodArrival* PodArrival)
{
	if (PodArrival == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ObjectiveHandler: DefendThePod was nullptr!"));
		return;
	}

	PodArrival->OnWaitForPodEventEnd.AddDynamic(this, &USGObjectiveHandlerSubSystem::UpdateCurrentGameObjective);
}

void USGObjectiveHandlerSubSystem::StartMission()
{
	ObjectiveToolTipWidget->HideVisitTerminal();
	if (GameObjectives.Num() > 0)
		CurrentObjective = GameObjectives[0];
	
	if (CurrentObjective == nullptr)
	{
		FString str = FString::Printf(TEXT("No more objectives!"));
		ObjectiveToolTipWidget->Display(FText::FromString(str));
		return;
	}
	
	CurrentObjective->OnStart();
	TerminalHUD->DisableStartButton();
	UGameplayStatics::PlaySound2D(this, MissionStartedSound);
	OnObjectiveStarted.Broadcast();
	OnObjectiveStartedWithType.Broadcast(CurrentObjective->GetObjectiveType());
}

// TODO: Ändra parameter till TSubscriptInterface<ISGObjectiveInterface> eller vad den nu hette...
void USGObjectiveHandlerSubSystem::UpdateCurrentGameObjective(UObject* ObjectiveInterfaceImplementor)
{
	EObjectiveType IncomingObjectiveType = EObjectiveType::EOT_None;
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
	if ((CurrentObjective->GetObjectiveType() & IncomingObjectiveType) == EObjectiveType::EOT_None)
		return;
	
	CurrentObjective->Update();
	
	if (CurrentObjective->IsCompleted())
	{
		ObjectiveToolTipWidget->GetCurrentHorizontalBoxObjective()->PlayAnimationValueCompleted();
		LastCompletedObjective = GetCurrentObjective();
		CurrentObjective->OnCompleted();
		UGameplayStatics::PlaySound2D(this, MissionCompletedSound);
		
		OnObjectiveCompleted.Broadcast();
		OnObjectiveCompletedWithType.Broadcast(IncomingObjectiveType);

		if (CurrentObjective->GetObjectiveType() != EObjectiveType::EOT_PodArrival)
		{
			RemoveCurrentObjective();
			ObjectiveToolTipWidget->ShowVisitTerminal();	
		}
		else
		{
			// Hack för att start FinalSweep efter PodArrival utan att behöva använda terminalen
			RemoveCurrentObjective();
			TerminalHUD->OnStartMission.Broadcast();
		}
	}
	else
	{
		ObjectiveToolTipWidget->GetCurrentHorizontalBoxObjective()->PlayUpdateValueAnimation();
	}
}

void USGObjectiveHandlerSubSystem::RemoveCurrentObjective()
{
	// Ta bort avklarat objective från Arrayen
	if (GameObjectives.Num() > 0)
	{
		ObjectivesCompleted.Add(CurrentObjective);
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


