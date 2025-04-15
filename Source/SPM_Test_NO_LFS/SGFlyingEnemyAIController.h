// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SGFlyingEnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class SPM_TEST_NO_LFS_API ASGFlyingEnemyAIController : public AAIController
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	class UBehaviorTree* AIBehaviorTree;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	APawn* PlayerPawn;
	
};
