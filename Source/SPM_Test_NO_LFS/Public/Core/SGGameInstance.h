// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Gear/Grapple/SGHUDGrapple.h"
#include "Objectives/SGObjectiveToolTipWidget.h"
#include "SaveGame/SGSaveGame.h"
#include "SaveGame/USGISaveGame.h"
#include "SGGameInstance.generated.h"

<<<<<<< HEAD
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameLoaded);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FONGameSaved);

class USaveGame;
=======
>>>>>>> parent of c75c5fc (ADD - PodArrival objective tillagt, ADD - Stor Refactor av Objectivesystem som nu använder ett subsystem och data assets för att sätta objectives data, ADD - Lagt in delar som blockar varje sektion av tornet och kan öppnas när ett visst objective är avklarat och stängas när spelaren går in i en triggerzone, dessa är komponenter som sätts på en mesh)
class USGTerminalWidget;
class USGSaveGame;
/**
 * 
 */
UCLASS()
class SPM_TEST_NO_LFS_API USGGameInstance : public UGameInstance, public ISGISaveGame
{
	GENERATED_BODY()
public:
	virtual void Init() override;

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

	UPROPERTY(EditAnywhere)
	class USGSaveGame* SavedData;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USGSaveGame> SaveGameClass;

	UFUNCTION(BlueprintCallable)
	virtual void LoadGameData_Implementation(bool Async) override;

	UFUNCTION(BlueprintCallable)
	virtual void SaveGameData_Implementation(bool Async) override;

	UFUNCTION(Blueprintable)
	class USGSaveGame* GetSaveGame() const;

	UPROPERTY(EditAnywhere)
	FString SlotName = TEXT("Saved Game 1");

	UFUNCTION()
	void OnSaveGameLoaded(const FString& TheSlotName, const int32 UserIndex, USaveGame* LoadedGameData);

	FPlayerStats PlayerStats;

	FOnGameLoaded OnGameLoaded;

	//FONGameSaved OnGameSaved;
	
};
