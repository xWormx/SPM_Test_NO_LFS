// Fill out your copyright notice in the Description page of Project Settings.
// comment
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "SPM_Test_NO_LFS.h"
#include "Objectives/SGGameObjectivesHandler.h"
#include "Engine/Engine.h"
#include "SGObjectiveHandlerSubSystem.generated.h"

class ASGObjectivePodArrival;
class USGDataAssetObjective;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnObjectiveStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnObjectiveCompleted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnObjectiveCompletedWithType, EObjectiveType, ObjectiveType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnObjectiveStartedWithType, EObjectiveType, ObjectiveType);

class USGGameInstance;
class ASGObjectiveBase;
/**
 * 
 */
UCLASS(Blueprintable)
class SPM_TEST_NO_LFS_API USGObjectiveHandlerSubSystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	void RegisterEnemy(ASGEnemyCharacter* Enemy);
	void RegisterCollectible(ASGPickUpObjectiveCollect* Collectible);
	void RegisterTerminalWidget(USGTerminalWidget* TerminalWidget);
	void RegisterDefendThePod(ASGObjectiveDefendThePod* DefendThePod);
	void RegisterPodArrival(ASGObjectivePodArrival* PodArrival);
	USGObjectiveToolTipWidget* GetObjectiveToolTipWidget() const {return ObjectiveToolTipWidget;}
	bool GetCurrentObjectiveIsActive() const { return CurrentObjective != nullptr; }
	EObjectiveType GetCurrentObjectiveType() const { return CurrentObjective->GetObjectiveType(); }
	ASGObjectiveBase* GetCurrentObjective() const { return CurrentObjective; }
	ASGObjectiveBase* GetLastCompletedObjective() const { return LastCompletedObjective; }
	EObjectiveType GetLastCompletedObjectiveType() const { return LastCompletedObjective->GetObjectiveType(); }
	TArray<ASGObjectiveBase*> GetAllObjectives() const { return GameObjectives; }

	
	FOnObjectiveStarted OnObjectiveStarted;
	FOnObjectiveCompleted OnObjectiveCompleted;
	FOnObjectiveCompletedWithType OnObjectiveCompletedWithType;

private:
	
	
	void OnWorldInitialized(const UWorld::FActorsInitializedParams& Params);

	void ReadObjectives();
	
	int ObjectiveCounter = 0;

	UPROPERTY(EditAnywhere, Category = Sound)
	USoundBase* MissionStartedSound;
	UPROPERTY(EditAnywhere, Category = Sound)
	USoundBase* MissionCompletedSound;
	
	UPROPERTY(VisibleAnywhere, Category = UPROPERTY)
	USGTerminalWidget* TerminalHUD;
/*
	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	TSubclassOf<USGObjectiveToolTipWidget> ObjectiveToolTipClass;
*/	
	UPROPERTY(VisibleAnywhere, Category = UPROPERTY)
	USGObjectiveToolTipWidget* ObjectiveToolTipWidget;
	
	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	TArray<ASGObjectiveBase*> GameObjectives;

	UPROPERTY(EditDefaultsOnly, Category = UPROPERTY)
	USGDataAssetObjective* ObjectiveDataAsset;
	
	UPROPERTY(VisibleAnywhere, Category = UPROPERTY)
	ASGObjectiveBase* CurrentObjective;
	UPROPERTY(VisibleAnywhere, Category = UPROPERTY)
	ASGObjectiveBase* LastCompletedObjective;

	UPROPERTY(VisibleAnywhere, Category = UPROPERTY)
	TArray<ASGEnemyCharacter*> TargetCharacters;

	UPROPERTY(VisibleAnywhere, Category = UPROPERTY)
	TArray<ASGPickUpObjectiveCollect*> TargetCollectibles;

		// Ta bort denna funktion, den är till för git test bara
	void RemoveThisFunctionLater();
	
	UFUNCTION()
	void UpdateCurrentGameObjective(UObject* ObjectiveInterfaceImplementor);

	UFUNCTION()
	void StartMission();

	void RemoveCurrentObjective();
};


