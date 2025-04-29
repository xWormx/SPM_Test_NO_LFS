// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objectives/SGObjectiveInterface.h"
#include "GameFramework/Character.h"
#include "SGEnemyCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDied, ASGEnemyCharacter*, DeadEnemy);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDiedObjective, UObject*, ObjectiveInterfaceImplementor);

UCLASS()
class SPM_TEST_NO_LFS_API ASGEnemyCharacter : public ACharacter, public ISGObjectiveInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASGEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void HandleDeath(float NewHealth);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual EObjectiveType GetObjectiveType() override { return EObjectiveType::EOT_KillAllEnemies; }
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnEnemyDied OnEnemyDied;
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnEnemyDiedObjective OnEnemyDiedObjective;
private:
	UPROPERTY()
	class USGHealthComponent* HealthComponent;

};
