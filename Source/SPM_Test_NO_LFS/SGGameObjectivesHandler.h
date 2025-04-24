// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGGameObjectivesHandler.generated.h"

class USGObjectiveToolTipWidget;
class ASGObjectiveBase;
class ASGEnemyCharacter;
class USGTerminalWidget;

UENUM(BlueprintType)
enum class EObjectiveType : uint8
{
	EOT_KillAllEnemies, EOT_CollectAndPlace, EOT_DefendThePod
};
class UButton;
UCLASS()
class SPM_TEST_NO_LFS_API ASGGameObjectivesHandler : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASGGameObjectivesHandler();

	void RegisterEnemy(ASGEnemyCharacter* Enemy);
	void RegisterTerminalWidget(USGTerminalWidget* TerminalWidget);
	USGObjectiveToolTipWidget* GetObjectiveToolTipWidget() {return ObjectiveToolTipWidget;}
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	int ObjectiveCounter = 0;
	UButton* StartButton;
	
	UPROPERTY(VisibleAnywhere, Category = UPROPERTY)
	USGTerminalWidget* TerminalHUD;

	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	TSubclassOf<USGObjectiveToolTipWidget> ObjectiveToolTipClass;
	
	UPROPERTY(VisibleAnywhere, Category = UPROPERTY)
	USGObjectiveToolTipWidget* ObjectiveToolTipWidget;
	
	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	TArray<EObjectiveType> GameObjectiveOrder;

	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	TArray<ASGObjectiveBase*> GameObjectives;

	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	ASGObjectiveBase* CurrentObjective;
	
	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	EObjectiveType CurrentObjectiveEnum;

	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	EObjectiveType StartObjective;

	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	int EnemiesKilled = 0;
	
	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	int NumberOfEnemiesToKill;

	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	int CurrentCollectedAmout = 0;
	
	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	int GoalCollectiblesAmount;

	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	bool bPodDefended = false;

	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	TSubclassOf<ASGEnemyCharacter> TargetCharacterClass;
	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	TArray<ASGEnemyCharacter*> TargetCharacters;
	
	UFUNCTION()
	void UpdateCurrentGameObjective(class ASGEnemyCharacter* Actor);

	UFUNCTION()
	void StartMission();
	
	void StartNextObjective(EObjectiveType NextObjectiveType);
};
