#include "Core/SGGameInstance.h"

#include "MoviePlayer.h"
#include "SPM_Test_NO_LFS.h"
#include "Core/SGUpgradeGuardSubsystem.h"
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
	if (!SaveGameClass)
	{
		EMMA_LOG(Error, TEXT("SaveGameClass is NULL! Save system will fail!"));
	}
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &USGGameInstance::UpdateUpgrades);

	CreateObjectiveToolTip();
	CreateHUDTerminal();

	OnDifficultyIncreased.AddDynamic(this, &USGGameInstance::IncreaseDifficultyLevel);
	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &USGGameInstance::StartLoadingScreen);

	EMMA_LOG(Warning, TEXT("Här är Emmas Log!"));
	BASIR_LOG(Warning, TEXT("Här är Basirs Log!"));
	CALLE_LOG(Warning, TEXT("Här är Calles Log!"));
	JOEL_LOG(Warning, TEXT("Här är Joels Log!"));
}

void USGGameInstance::Shutdown()
{
	FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);

	OnDifficultyIncreased.Clear();

	Super::Shutdown();
}

void USGGameInstance::UpdateUpgrades([[maybe_unused]] UWorld* World)
{
	USGUpgradeSubsystem* UpgradeSubsystem = GetSubsystem<USGUpgradeSubsystem>();
	USGUpgradeGuardSubsystem* UpgradeGuard = GetSubsystem<USGUpgradeGuardSubsystem>();

	if (UpgradeGuard)
	{
		EMMA_LOG(Warning, TEXT("☀️USGGameInstance::UpdateUpgrades: Resetting upgrade guard count..."));
		UpgradeGuard->ResetCount();
	}
	if (!DoesSaveGameExist())
	{
		EMMA_LOG(Warning, TEXT("☀️USGGameInstance::UpdateUpgrades: No save data found"));
		return;
	}

	EMMA_LOG(Warning, TEXT("☀️USGGameInstance::UpdateUpgrades: Loading persistent upgrades from save data..."));
	FSGSavedAttributes UpgradeSaves = SavedData->UpgradeSystemSavedAttributes;
	if (UpgradeSubsystem)
	{
		UpgradeSubsystem->LoadSavedAttributes(UpgradeSaves);
	}
	if (UpgradeGuard)
	{
		UpgradeGuard->SetCount(UpgradeSaves.Orbs);
	}
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
	if (!SavedData || !SavedData->PlayerStats.bSaveGameExists || SlotName.IsEmpty())
	{
		return false;
	}
	return UGameplayStatics::DoesSaveGameExist(SlotName, 0);
}

void USGGameInstance::IncreaseDifficultyLevel([[maybe_unused]] int Difficulty)
{
	if (const USGUpgradeSubsystem* UpgradeSubsystem = GetSubsystem<USGUpgradeSubsystem>())
	{
		UpgradeSubsystem->UpgradeByRow(TEXT("EnemyHealth"));
		UpgradeSubsystem->UpgradeByRow(TEXT("EnemyDamage"));
	}
}

void USGGameInstance::LoadGameData(bool bAsync)
{
	if (!SaveGameClass)
	{
		EMMA_LOG(Error, TEXT("SaveGameClass is NULL! Cannot load or create save game."));
		return;
	}
	if (!UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		SavedData = Cast<USGSaveGame>(UGameplayStatics::CreateSaveGameObject(SaveGameClass));
		EMMA_LOG(Warning, TEXT("☀️USGGameInstance::LoadGameData: No save game found, creating new save game object."));
	}
	else
	{
		if (bAsync)
		{
			FAsyncLoadGameFromSlotDelegate AsyncLoadGameFromSlotDelegate;
			AsyncLoadGameFromSlotDelegate.BindUObject(this, &USGGameInstance::OnSaveGameLoaded);
			UGameplayStatics::AsyncLoadGameFromSlot( SlotName,0,AsyncLoadGameFromSlotDelegate);
		}
		else
		{
			USaveGame* LoadedGameData = UGameplayStatics::LoadGameFromSlot(SlotName, 0);
			SavedData = Cast<USGSaveGame>(LoadedGameData);
			EMMA_LOG(Warning, TEXT("☀️USGGameInstance::LoadGameData: Loaded game data from slot %s."), *SlotName);
			for (FSGSavedAttributeEntry SomeAttribute : SavedData->UpgradeSystemSavedAttributes.SomeAttributes)
			{
				EMMA_LOG(Warning, TEXT("☀️USGGameInstance::LoadGameData: Loaded Attribute: %s"), *SomeAttribute.ClassNameKey);
			}
		}
	}
	EMMA_LOG(Warning, TEXT("☀️USGGameInstance::LoadGameData: Save game data loaded successfully."));
}

void USGGameInstance::SaveGameData(bool bAsync)
{
	if (!SavedData)
	{
		EMMA_LOG(Error, TEXT("😭Cannot save game: SavedData is NULL"));
		return;
	}
	if (bAsync)
	{
		FAsyncSaveGameToSlotDelegate SavedDelegate;
		SavedDelegate.BindUObject(this, &USGGameInstance::OnGameSaved);
	    UGameplayStatics::AsyncSaveGameToSlot(SavedData, SlotName, 0, SavedDelegate);

	}
	else
	{
		const bool bSuccess = UGameplayStatics::SaveGameToSlot(SavedData, SlotName, 0);
		if (!bSuccess)
		{
			EMMA_LOG(Error, TEXT("😭Failed to save game to slot %s"), *SlotName);
		}
	}
}

void USGGameInstance::OnGameSaved(const FString& TheSaveSlot, const int32 UserIndex, bool bSuccess)
{
	if (bSuccess)
	{
		EMMA_LOG(Warning, TEXT("✅ Successfully saved game to slot %s"), *TheSaveSlot);
	}
	else
	{
		EMMA_LOG(Error, TEXT("❌Failed to save game to slot %s"), *TheSaveSlot);
	}
}
void USGGameInstance::OnSaveGameLoaded(const FString& TheSlotName, const int32 UserIndex, USaveGame* LoadedGameData)
{
	USGSaveGame* TypedSaveGame = Cast<USGSaveGame>(LoadedGameData);
	if (TypedSaveGame)
	{
		SavedData = TypedSaveGame;
		EMMA_LOG(Warning, TEXT("✅ Save game loaded successfully from slot %s"), *TheSlotName);
		OnGameLoaded.Broadcast();
	}
	else
	{
		BASIR_LOG(Warning, TEXT("❌SaveGame is not of type USGSaveGame!"));
		SavedData = Cast<USGSaveGame>(UGameplayStatics::CreateSaveGameObject(SaveGameClass));
	}
	/*if (Cast<USGSaveGame>(LoadedGameData))
	{
		OnGameLoaded.Broadcast();
	}
	else
	{
		BASIR_LOG(Warning, TEXT("SaveGame is not of type USGSaveGame!"));
	}*/
}

void USGGameInstance::ResetSavedGame()
{
	if (!SavedData && SaveGameClass)
	{
		SavedData = Cast<USGSaveGame>(UGameplayStatics::CreateSaveGameObject(SaveGameClass));
	}

	if (SavedData)
	{
		SavedData->PlayerStats.bSaveGameExists = false;
		SavedData->SavedObjectives.ShouldLoad = false;
		SavedData->SpawnManagerSavedData.bSaveGameExists = false;
	}
}

FSGSavedAttributes USGGameInstance::GetSavedUpgradeAttributes() const
{
	if (!SavedData)
	{
		EMMA_LOG(Warning, TEXT("💀GetSavedUpgradeAttributes: SavedData is NULL!"));
		return FSGSavedAttributes();
	}
	return SavedData->UpgradeSystemSavedAttributes;
}

FPlayerStats USGGameInstance::GetSavedPlayerStats() const
{
	if (!SavedData)
	{
		EMMA_LOG(Warning, TEXT("💀GetSavedPlayerStats: SavedData is NULL!"));
		return FPlayerStats();
	}
	return SavedData->PlayerStats;
}

FObjectiveSaveData USGGameInstance::GetSavedObjectives() const
{
	if (!SavedData)
	{
		EMMA_LOG(Warning, TEXT("💀GetSavedObjectives: SavedData is NULL!"));
		return FObjectiveSaveData();
	}
	return SavedData->SavedObjectives;
}

USGSaveGame* USGGameInstance::GetSaveGame() const
{
	if (!SavedData)
	{
		EMMA_LOG(Warning, TEXT("💀GetSaveGame: SavedData is NULL!"));
		return nullptr;
	}
	return SavedData;
}

FSpawnManagerSavedData USGGameInstance::GetSpawnManagerSavedData() const
{
	if (!SavedData)
	{
		EMMA_LOG(Warning, TEXT("💀GetSpawnManagerSavedData: SavedData is NULL!"));
		return FSpawnManagerSavedData();
	}
	return SavedData->SpawnManagerSavedData;
}

void USGGameInstance::SaveGame(FPlayerStats PlayerStats, FSGSavedAttributes UpgradeStats, FObjectiveSaveData SavedObjectives, FSpawnManagerSavedData SpawnManagerSavedData,const bool bAsync)
{	if (!SavedData)
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

	for (auto [ClassNameKey, PersistentUpgrades] : SavedData->UpgradeSystemSavedAttributes.SomeAttributes)
	{
		EMMA_LOG(Warning, TEXT("Saving Attribute: %s"), *ClassNameKey);
	}
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

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!PlayerController)
	{
		EMMA_LOG(Warning, TEXT("💀PlayerController is NULL, cannot collect player stats."));
		return;
	}


	ASGPlayerCharacter* PlayerCharacter = Cast<ASGPlayerCharacter>(World->GetFirstPlayerController()->GetPawn());
	if (!PlayerCharacter)
	{
		EMMA_LOG(Warning, TEXT("💀PlayerCharacter is NULL, cannot collect player stats."));
		return;
	}

	USGUpgradeSubsystem* UpgradesSubSystem = GetSubsystem<USGUpgradeSubsystem>();
	USGUpgradeGuardSubsystem* UpgradeGuard = GetSubsystem<USGUpgradeGuardSubsystem>();
	if (!UpgradesSubSystem || !UpgradeGuard)
	{
		BASIR_LOG(Error, TEXT("UpgradesSystem or UpgradeGuard is Null"));
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
	FSGSavedAttributes UpgradeAttributes = UpgradesSubSystem->GetSavedAttributes();
	UpgradeAttributes.Orbs = UpgradeGuard->GetCount();
	FObjectiveSaveData SavedObjectives = SGObjectiveSystem->GetSaveGameData();
	FSpawnManagerSavedData SpawnManagerSavedData = SGSpawnManager->GetSaveData();

	SaveGame(PlayerStats, UpgradeAttributes, SavedObjectives, SpawnManagerSavedData, bAsync);
	
}
