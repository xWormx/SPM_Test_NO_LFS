// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGObjectiveBase.h"
#include "GameFramework/Actor.h"
#include "SGGameObjectivesHandler.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnObjectiveStarted);
DECLARE_MULTICAST_DELEGATE(FOnObjectiveCompleted);

enum class EObjectiveType : uint8;
class ASGObjectiveDefendThePod;
class ASGPickUpObjectiveCollect;
class USGObjectiveToolTipWidget;
class ASGObjectiveBase;
class ASGEnemyCharacter;
class USGTerminalWidget;

class UButton;
UCLASS()
class SPM_TEST_NO_LFS_API ASGGameObjectivesHandler : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASGGameObjectivesHandler();

	void RegisterEnemy(ASGEnemyCharacter* Enemy);
	void RegisterCollectible(ASGPickUpObjectiveCollect* Collectible);
	void RegisterTerminalWidget(USGTerminalWidget* TerminalWidget);
	void RegisterDefendThePod(ASGObjectiveDefendThePod* DefendThePod);
	USGObjectiveToolTipWidget* GetObjectiveToolTipWidget() const {return ObjectiveToolTipWidget;}
	bool GetCurrentObjectiveIsActive() const { return CurrentObjective != nullptr; }
	EObjectiveType GetCurrentObjectiveType() const { return CurrentObjective->GetObjectiveType(); }
	ASGObjectiveBase* GetCurrentObjective() const { return CurrentObjective; }
	ASGObjectiveBase* GetLastCompletedObjective() const { return LastCompletedObjective; }
	EObjectiveType GetLastCompletedObjectiveType() const { return LastCompletedObjective->GetObjectiveType(); }
	TArray<ASGObjectiveBase*> GetAllObjectives() const { return GameObjectives; }
	FOnObjectiveStarted OnObjectiveStarted;
	FOnObjectiveCompleted OnObjectiveCompleted;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	int ObjectiveCounter = 0;

	UPROPERTY(EditAnywhere, Category = Sound)
	USoundBase* MissionStartedSound;
	UPROPERTY(EditAnywhere, Category = Sound)
	USoundBase* MissionCompletedSound;
	
	UPROPERTY(VisibleAnywhere, Category = UPROPERTY)
	USGTerminalWidget* TerminalHUD;

	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	TSubclassOf<USGObjectiveToolTipWidget> ObjectiveToolTipClass;
	
	UPROPERTY(VisibleAnywhere, Category = UPROPERTY)
	USGObjectiveToolTipWidget* ObjectiveToolTipWidget;
	
	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	float ObjectiveToolTipFadeFactor = 1.0f;
	
	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	TArray<ASGObjectiveBase*> GameObjectives;

	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	ASGObjectiveBase* CurrentObjective;
	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	ASGObjectiveBase* LastCompletedObjective;

	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	TSubclassOf<ASGEnemyCharacter> TargetCharacterClass;
	UPROPERTY(VisibleAnywhere, Category = UPROPERTY)
	TArray<ASGEnemyCharacter*> TargetCharacters;

	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	TSubclassOf<ASGPickUpObjectiveCollect> TargetCollectibleClass;
	UPROPERTY(VisibleAnywhere, Category = UPROPERTY)
	TArray<ASGPickUpObjectiveCollect*> TargetCollectibles;
	
	UFUNCTION()
	void UpdateCurrentGameObjective(UObject* ObjectiveInterfaceImplementor);

	UFUNCTION()
	void StartMission();

	void RemoveCurrentObjective();
	
};
