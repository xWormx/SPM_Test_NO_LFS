#include "Core/SGGameInstance.h"
#include "SPM_Test_NO_LFS.h"
#include "Core/SGUpgradeSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Objectives/SGTerminalWidget.h"
#include "SaveGame/SGSaveGame.h"

void USGGameInstance::Init()
{
	Super::Init();

	LoadGameData(false);
	ResetSavedGame();

	//GetSubsystem<USGUpgradeSubsystem>()->LoadPersistentUpgrades(SavedData->UpgradeSystemSavedAttributes);

	CreateObjectiveToolTip();
	CreateHUDTerminal();

	OnDifficultyIncreased.AddDynamic(this, &USGGameInstance::IncreaseDifficultyLevel);
	
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

inline void USGGameInstance::CreateHUDTerminal()
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

void USGGameInstance::SaveGame(struct FPlayerStats PlayerStats,
	struct FSGSavedAttributes UpgradeStats,
	struct FObjectiveSaveData SavedObjectives,
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
	SaveGameData(bAsync);
}
