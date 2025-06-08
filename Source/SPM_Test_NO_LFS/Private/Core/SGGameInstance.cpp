#include "Core/SGGameInstance.h"

#include "MoviePlayer.h"
#include "SPM_Test_NO_LFS.h"
#include "Core/SGUpgradeSubsystem.h"
#include "Enemies/Managers/SGEnemySpawnManager.h"
#include "Kismet/GameplayStatics.h"
#include "Objectives/SGTerminalWidget.h"
#include "Player/SGPlayerCharacter.h"
#include "Player/SGPlayerController.h"
#include "SaveGame/SGSaveGame.h"

void USGGameInstance::Init()
{
	Super::Init();

	LoadGameData(false);
	//ResetSavedGame();

	CreateObjectiveToolTip();
	CreateHUDTerminal();

	OnDifficultyIncreased.AddDynamic(this, &USGGameInstance::IncreaseDifficultyLevel);

	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &USGGameInstance::StartLoadingScreen);

	EMMA_LOG(Warning, TEXT("Här är Emmas Log!"));
	BASIR_LOG(Warning, TEXT("Här är Basirs Log!"));
	CALLE_LOG(Warning, TEXT("Här är Calles Log!"));
	JOEL_LOG(Warning, TEXT("Här är Joels Log!"));
}

void USGGameInstance::CreateObjectiveToolTip()
{
	if (ObjectiveToolTipClass)
	{
		if (ObjectiveToolTipWidget)
		{
			ObjectiveToolTipWidget->DeInitialize();
			ObjectiveToolTipWidget = nullptr;
		}
		ObjectiveToolTipWidget = Cast<USGObjectiveToolTipWidget>(CreateWidget(this, ObjectiveToolTipClass));
	}
}

void USGGameInstance::CreateHUDTerminal()
{
	if (HUDTerminalClass)
	{
		if (HUDTerminal)
		{
			HUDTerminal->RemoveFromParent();
			HUDTerminal = nullptr;
		}
		HUDTerminal = Cast<USGTerminalWidget>(CreateWidget(this, HUDTerminalClass));
	}
}

void USGGameInstance::SetTerminalWidget(USGTerminalWidget* InWidget)
{
	HUDTerminal = InWidget;
}

void USGGameInstance::SetObjectiveTooltipWidget(USGObjectiveToolTipWidget* InObjectiveTooltipWidget)
{ ObjectiveToolTipWidget = InObjectiveTooltipWidget; }

USGTerminalWidget* USGGameInstance::GetTerminalWidget() const
{
	return HUDTerminal;
}

USGObjectiveToolTipWidget* USGGameInstance::GetObjectiveTooltipWidget() const
{
	return ObjectiveToolTipWidget;
}

void USGGameInstance::StartLoadingScreen([[maybe_unused]] const FString& MapName)
{
	if (!IsRunningDedicatedServer())
	{
		UUserWidget* const LoadingWidget = CreateWidget<UUserWidget>(this, LoadingWidgetClass, TEXT("LoadingScreen"));
		TSharedRef<SWidget> LoadingSWidgetPtr = LoadingWidget->TakeWidget();

		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.WidgetLoadingScreen = LoadingSWidgetPtr;
		LoadingScreen.bAllowInEarlyStartup = false;
		LoadingScreen.PlaybackType = MT_Normal;
		LoadingScreen.bAllowEngineTick = false;
		LoadingScreen.bWaitForManualStop = false;
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = true;
		LoadingScreen.MinimumLoadingScreenDisplayTime = 1.f;

		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
	}
}

void USGGameInstance::LoadLevel(const FName& MapName)
{
	GEngine->GameViewport->RemoveAllViewportWidgets();
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	UGameplayStatics::OpenLevel(this, MapName);
}

bool USGGameInstance::DoesSaveGameExist() const
{
	return UGameplayStatics::DoesSaveGameExist(SlotName, 0);
}

void USGGameInstance::IncreaseDifficultyLevel([[maybe_unused]] int Difficulty)
{
	if (const USGUpgradeSubsystem* UpgradeSubsystem = GetSubsystem<USGUpgradeSubsystem>())
	{
		UpgradeSubsystem->ModifyAttributeByRow(TEXT("EnemyHealth"));
		UpgradeSubsystem->ModifyAttributeByRow(TEXT("EnemyDamage"));
	}
}

void USGGameInstance::LoadGameData(bool bAsync)
{
	if (!UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		SavedData = Cast<USGSaveGame>(UGameplayStatics::CreateSaveGameObject(SaveGameClass));
	}
	else
	{
		if (bAsync)
		{
			FAsyncLoadGameFromSlotDelegate AsyncLoadGameFromSlotDelegate;
			AsyncLoadGameFromSlotDelegate.BindUObject(this, &USGGameInstance::OnSaveGameLoaded);
			UGameplayStatics::AsyncLoadGameFromSlot( SlotName,
				0,
				AsyncLoadGameFromSlotDelegate
				);
		}
		else
		{
			USaveGame* LoadedGameData = UGameplayStatics::LoadGameFromSlot(SlotName, 0);

			SavedData = Cast<USGSaveGame>(LoadedGameData);
		}
	}
}

void USGGameInstance::SaveGameData(bool bAsync)
{
	if (bAsync)
	{
		UGameplayStatics::AsyncSaveGameToSlot(SavedData, SlotName, 0); 
	}
	else
	{
		UGameplayStatics::SaveGameToSlot(SavedData, SlotName, 0);
	}
}

void USGGameInstance::OnSaveGameLoaded(const FString& TheSlotName, const int32 UserIndex, USaveGame* LoadedGameData) const
{
	if (Cast<USGSaveGame>(LoadedGameData))
	{
		OnGameLoaded.Broadcast();
	}
	else
	{
		BASIR_LOG(Warning, TEXT("SaveGame is not of type USGSaveGame!"));
	}
}

void USGGameInstance::ResetSavedGame()
{
	if (SavedData)
	{
		SavedData->PlayerStats.bSaveGameExists = false;
		SavedData->SavedObjectives.ShouldLoad = false;
		SavedData->SpawnManagerSavedData.bSaveGameExists = false;
	}
}

struct FSGSavedAttributes USGGameInstance::GetSavedUpgradeAttributes() const
{
	return SavedData->UpgradeSystemSavedAttributes;
}

struct FPlayerStats USGGameInstance::GetSavedPlayerStats() const
{
	return SavedData->PlayerStats;
}

struct FObjectiveSaveData USGGameInstance::GetSavedObjectives() const
{
	return SavedData->SavedObjectives;
}

USGSaveGame* USGGameInstance::GetSaveGame() const
{
	return SavedData;
}

struct FSpawnManagerSavedData USGGameInstance::GetSpawnManagerSavedData() const
{
	return SavedData->SpawnManagerSavedData;
}

void USGGameInstance::SaveGame(struct FPlayerStats PlayerStats,
	struct FSGSavedAttributes UpgradeStats,
	struct FObjectiveSaveData SavedObjectives,
	struct FSpawnManagerSavedData SpawnManagerSavedData,
	const bool bAsync)
{
	if (!SavedData)
	{
		BASIR_LOG(Warning, TEXT("SavedData is NULL!"));
		SavedData = Cast<USGSaveGame>(UGameplayStatics::CreateSaveGameObject(SaveGameClass));
		
		if (!SavedData)
		{
			BASIR_LOG(Warning, TEXT("SavedData is NULL!, aborting save."));
			return;
		}
	}
	SavedData->PlayerStats = PlayerStats;
	SavedData->UpgradeSystemSavedAttributes = UpgradeStats;
	SavedData->SavedObjectives = SavedObjectives;
	SavedData->SpawnManagerSavedData = SpawnManagerSavedData;
	SaveGameData(bAsync);
}

void USGGameInstance::CollectAndSave(const bool bAsync)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		EMMA_LOG(Warning, TEXT("💀World is NULL, cannot collect player stats."));
		return;
	}
	
	ASGPlayerCharacter* PlayerCharacter = Cast<ASGPlayerCharacter>(World->GetFirstPlayerController()->GetPawn());
	if (!PlayerCharacter)
	{
		EMMA_LOG(Warning, TEXT("💀PlayerCharacter is NULL, cannot collect player stats."));
		return;
	}

	USGUpgradeSubsystem* UpgradesSubSystem = GetSubsystem<USGUpgradeSubsystem>();
	if (!UpgradesSubSystem)
	{
		BASIR_LOG(Error, TEXT("UpgradesSystem is Null"));
		return;
	}

	USGObjectiveHandlerSubSystem* SGObjectiveSystem = World->GetSubsystem<USGObjectiveHandlerSubSystem>();
	if (!SGObjectiveSystem)
	{
		BASIR_LOG(Error, TEXT("No Objective System, Null Value"));
		return;
	}

	ASGEnemySpawnManager* SGSpawnManager = PlayerCharacter->GetCurrentSpawnManager();
	if (!SGSpawnManager)
	{
		BASIR_LOG(Error, TEXT("No Spawn Manager, Null Value"));
		return;
	}

	FPlayerStats PlayerStats = PlayerCharacter->GetPlayerStats();
	FSGSavedAttributes UpgradeAttributes = UpgradesSubSystem->GetPersistentUpgrades();
	FObjectiveSaveData SavedObjectives = SGObjectiveSystem->GetSaveGameData();
	FSpawnManagerSavedData SpawnManagerSavedData = SGSpawnManager->GetSaveData();

	SaveGame(PlayerStats, UpgradeAttributes, SavedObjectives, SpawnManagerSavedData, bAsync);
	
}
