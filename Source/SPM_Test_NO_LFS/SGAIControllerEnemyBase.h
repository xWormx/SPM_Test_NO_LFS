// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SGAIControllerEnemyBase.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class SPM_TEST_NO_LFS_API ASGAIControllerEnemyBase : public AAIController
{
	GENERATED_BODY()
protected:
	ASGAIControllerEnemyBase();
	
	virtual void BeginPlay() override;

	virtual bool CanAttackTarget() const;

	virtual void HandleMovement();

	UPROPERTY(EditAnywhere, Category= "Combat", meta = (AllowPrivateAccess = true))
	float AttackRange;

	UPROPERTY(EditAnywhere,  Category= "Combat", meta = (AllowPrivateAccess = true))
	AActor* AttackTarget;

	UPROPERTY(EditAnywhere, Category= "Movement",  meta = (AllowPrivateAccess = true))
	float AcceptanceRadius;

	UPROPERTY(EditAnywhere,  Category= "Movement", meta = (AllowPrivateAccess = true))
	float RetreatDistance;


public:
	virtual void Tick(float DeltaTime) override;
	
};
