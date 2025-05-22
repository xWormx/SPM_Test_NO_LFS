// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/SGGameInstance.h"

#include "SPM_Test_NO_LFS.h"
#include "Core/SGUpgradeSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Objectives/SGTerminalWidget.h"
#include "SaveGame/SGSaveGame.h"

void USGGameInstance::Init()
{
	Super::Init();

	CreateObjectiveToolTip();
	CreateHUDTerminal();
	
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

void USGGameInstance::IncreaseDifficultyLevel(int Difficulty)
{
	if (const USGUpgradeSubsystem* UpgradeSubsystem = GetSubsystem<USGUpgradeSubsystem>())
	{
		UpgradeSubsystem->ModifyAttributeByRow(TEXT("EnemyHealth"));
		UpgradeSubsystem->ModifyAttributeByRow(TEXT("EnemyDamage"));
	}
}

void USGGameInstance::LoadGameData_Implementation(bool Async)
{
	ISGISaveGame::LoadGameData_Implementation(Async);

	if (!UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		SavedData = Cast<USGSaveGame>(UGameplayStatics::CreateSaveGameObject(SaveGameClass));
	}
	else
	{
		if (Async)
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


void USGGameInstance::SaveGameData_Implementation(bool Async)
{
	ISGISaveGame::SaveGameData_Implementation(Async);

	if (Async)
	{
		UGameplayStatics::AsyncSaveGameToSlot(SavedData, SlotName, 0); 
	}
	else
	{
		UGameplayStatics::SaveGameToSlot(SavedData, SlotName, 0);
	}
}


void USGGameInstance::OnSaveGameLoaded(const FString& TheSlotName, const int32 UserIndex, USaveGame* LoadedGameData)
{
	if (USGSaveGame* SaveGame = Cast<USGSaveGame>(LoadedGameData))
	{
		//PlayerStats = SaveGame->PlayerStats;
		OnGameLoaded.Broadcast();
	}
	else
	{
		BASIR_LOG(Warning, TEXT("SaveGame is not of type USGSaveGame!"));
	}
}

class USGSaveGame* USGGameInstance::GetSaveGame() const
{
	return SavedData;
}
