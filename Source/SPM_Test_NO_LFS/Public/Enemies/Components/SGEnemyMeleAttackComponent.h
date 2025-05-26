// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGEnemyAttackComponentBase.h"
#include "SGEnemyMeleAttackComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SPM_TEST_NO_LFS_API USGEnemyMeleAttackComponent : public USGEnemyAttackComponentBase
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USGEnemyMeleAttackComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	virtual void StartAttack(AActor* Target) override;

protected:

	virtual void PerformAttack(AActor* Target) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio", meta=(AllowPrivateAccess="true"))
	class USoundBase* MeleeSound;
};
