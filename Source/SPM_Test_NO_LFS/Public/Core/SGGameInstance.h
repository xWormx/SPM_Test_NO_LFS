// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Gear/Grapple/SGHUDGrapple.h"
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
	void SetTerminalWidget(USGTerminalWidget* InWidget) { HUDTerminal = InWidget; }
	void SetHUDGrapple(USGHUDGrapple* InHUDGrapple) { HUDGrapple = InHUDGrapple; }
	void SetObjectiveTooltipWidget(USGObjectiveToolTipWidget* InObjectiveTooltipWidget) { ObjectiveToolTipWidget = InObjectiveTooltipWidget; }
	USGTerminalWidget* GetTerminalWidget() { return HUDTerminal; }
	USGHUDGrapple* GetHUDGrapple() {	return HUDGrapple; }
	USGObjectiveToolTipWidget* GetObjectiveTooltipWidget() { return ObjectiveToolTipWidget; }
	
private:

	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	TSubclassOf<USGTerminalWidget> HUDTerminalClass;

	UPROPERTY(VisibleAnywhere, Category = UPROPERTY)
	USGTerminalWidget* HUDTerminal;

	UPROPERTY(VisibleAnywhere, Category = UPROPERTY)
	USGHUDGrapple* HUDGrapple;

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

	UFUNCTION(Blueprintable)
	class USGSaveGame* GetSaveGame() const;

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
