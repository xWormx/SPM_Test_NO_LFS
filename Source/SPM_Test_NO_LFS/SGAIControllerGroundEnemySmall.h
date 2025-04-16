// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SGAIControllerGroundEnemySmall.generated.h"

/**
 * 
 */
UCLASS()
class SPM_TEST_NO_LFS_API ASGAIControllerGroundEnemySmall : public AAIController
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	float AttackRange = 800.f;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	float AcceptanceRadius = 600.f;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	float RetreatDistance = 500.0f;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	APawn* PlayerPawn;

	bool CanShootPlayer() const;

	void HandleMovement();
	
};
