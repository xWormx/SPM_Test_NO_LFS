// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGEnemyAttackComponentBase.h"
#include "SGEnemyShootAttackComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SPM_TEST_NO_LFS_API USGEnemyShootAttackComponent : public USGEnemyAttackComponentBase
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USGEnemyShootAttackComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	virtual void StartAttack(AActor* Target) override;

protected:
	virtual void PerformAttack(AActor* Target) override;

	
};
