// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGAIControllerEnemyBase.h"
#include "SGAIControllerEnemyFlying.generated.h"

/**
 * 
 */
UCLASS()
class SPM_TEST_NO_LFS_API ASGAIControllerEnemyFlying : public ASGAIControllerEnemyBase
{
	GENERATED_BODY()

protected:
	ASGAIControllerEnemyFlying();

	virtual void BeginPlay() override;

	virtual void HandleMovement() override;

	virtual void FlyTowardsTarget();

public:
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, Category = "Movement" ,meta = (AllowPrivateAccess = true))
	float DetectionRadius = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Movement" ,meta = (AllowPrivateAccess = true))
	float HoverAmplitude = 200.f;

	UPROPERTY(EditAnywhere, Category = "Movement" ,meta = (AllowPrivateAccess = true))
	float HoverSpeed = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Movement" ,meta = (AllowPrivateAccess = true))
	float DesiredDistance = 300.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float HoverInterpSpeed = 5.0f;

};
