#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Objectives/SGObjectiveToolTipWidget.h"

#include "SGGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameLoaded);

class USGTerminalWidget;
class USGSaveGame;

UCLASS()
class SPM_TEST_NO_LFS_API USGGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	virtual void Init() override;
	void CreateObjectiveToolTip();
	void CreateHUDTerminal();
	void SetTerminalWidget(USGTerminalWidget* InWidget);
	void SetObjectiveTooltipWidget(USGObjectiveToolTipWidget* InObjectiveTooltipWidget);
	USGTerminalWidget* GetTerminalWidget() const;
	USGObjectiveToolTipWidget* GetObjectiveTooltipWidget() const;

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
	UFUNCTION()
	void IncreaseDifficultyLevel(int Difficulty);

	//Saving functionality
		//Functions
	UFUNCTION(BlueprintCallable)
	void LoadGameData(bool bAsync);


	UFUNCTION(BlueprintCallable)
	virtual void SaveGameData(bool bAsync);

	UFUNCTION(BlueprintCallable)
	class USGSaveGame* GetSaveGame() const;

	UFUNCTION(BlueprintCallable)
	void SavePlayerStats(struct FPlayerStats PlayerStats, const bool bAsync);

	UFUNCTION()
	void OnSaveGameLoaded(const FString& TheSlotName, const int32 UserIndex, USaveGame* LoadedGameData) const;

		//Other
	FOnGameLoaded OnGameLoaded;
	
	UPROPERTY(EditAnywhere)
	USGSaveGame* SavedData;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USGSaveGame> SaveGameClass;
	
	UPROPERTY(EditAnywhere)
	FString SlotName = TEXT("Saved Game 1");
};
