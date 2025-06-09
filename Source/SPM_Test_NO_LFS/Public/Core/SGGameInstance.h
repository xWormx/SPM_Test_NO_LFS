#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameFramework/SaveGame.h"
#include "Objectives/SGObjectiveToolTipWidget.h"

#include "SGGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDifficultyIncreased, int, NewDifficultLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameLoaded);

class USGTerminalWidget;
class USGSaveGame;

UCLASS()
class SPM_TEST_NO_LFS_API USGGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	virtual void Init() override;
	void Shutdown();
	UFUNCTION()
	void UpdateUpgrades(UWorld* World);
	void CreateObjectiveToolTip();
	void CreateHUDTerminal();
	void SetTerminalWidget(USGTerminalWidget* InWidget);
	void SetObjectiveTooltipWidget(USGObjectiveToolTipWidget* InObjectiveTooltipWidget);
	USGTerminalWidget* GetTerminalWidget() const;
	USGObjectiveToolTipWidget* GetObjectiveTooltipWidget() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Loading Screen")
	TSubclassOf<UUserWidget> LoadingWidgetClass;

	UFUNCTION()
	void StartLoadingScreen(const FString& MapName);

	UFUNCTION()
	void LoadLevel(const FName& MapName);

	bool DoesSaveGameExist() const;
private:

	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	TSubclassOf<USGTerminalWidget> HUDTerminalClass;

	UPROPERTY(VisibleAnywhere, Category = UPROPERTY)
	USGTerminalWidget* HUDTerminal;

	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	TSubclassOf<USGObjectiveToolTipWidget> ObjectiveToolTipClass;
	
	UPROPERTY(VisibleAnywhere, Category = UPROPERTY)
	USGObjectiveToolTipWidget* ObjectiveToolTipWidget;

public:
	FOnDifficultyIncreased OnDifficultyIncreased;
	
	UFUNCTION()
	void IncreaseDifficultyLevel(int Difficulty);

	//Saving functionality
	UFUNCTION(BlueprintCallable)
	void LoadGameData(bool bAsync);

	UFUNCTION(BlueprintCallable)
	virtual void SaveGameData(bool bAsync);
	void OnGameSaved(const FString& TheSaveSlot, int32 UserIndex, bool bSuccess);

	UFUNCTION(BlueprintCallable)
	class USGSaveGame* GetSaveGame() const;

	UFUNCTION(BlueprintCallable)
	void SaveGame(struct FPlayerStats PlayerStats, struct FSGSavedAttributes UpgradeStats, struct FObjectiveSaveData SavedObjectives, struct FSpawnManagerSavedData SpawnManagerSavedData,const bool bAsync);

	void CollectAndSave(const bool bAsync);

	/*UFUNCTION(BlueprintCallable)
	void SaveUpgradeStats(struct FSGSavedAttributes UpgradesStats, const bool bAsync);*/

	UFUNCTION()
	void OnSaveGameLoaded(const FString& TheSlotName, const int32 UserIndex, USaveGame* LoadedGameData);

	UFUNCTION(BlueprintCallable)
	struct FPlayerStats GetSavedPlayerStats() const;

	UFUNCTION(BlueprintCallable)
	struct FSGSavedAttributes GetSavedUpgradeAttributes() const;

	UFUNCTION(BlueprintCallable)
	struct FObjectiveSaveData GetSavedObjectives() const;

	UFUNCTION(BlueprintCallable)
	struct FSpawnManagerSavedData GetSpawnManagerSavedData() const;

	UFUNCTION(BlueprintCallable)
	void ResetSavedGame();

		//Other
	FOnGameLoaded OnGameLoaded;
	
	UPROPERTY(EditAnywhere)
	USGSaveGame* SavedData;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USGSaveGame> SaveGameClass;
	
	UPROPERTY(EditAnywhere)
	FString SlotName = TEXT("Saved Game 1");

	UPROPERTY(EditAnywhere)
	class USGUpgradeSubsystem* UpgradeSubSystem;

	FDelegateHandle UpdateUpgradesHandle;
};
