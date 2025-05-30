#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SGAIControllerEnemy.generated.h"

UCLASS()
class SPM_TEST_NO_LFS_API ASGAIControllerEnemy : public AAIController
{
	GENERATED_BODY()

public:
	
	ASGAIControllerEnemy();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	virtual void SetInitialValues();

private:
	//Behavior Tree
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* BehaviorTree;
	
	//float values
	UPROPERTY(EditAnywhere, Category= "Movement",  meta = (AllowPrivateAccess = true))
	float AttackRange = 0;

	UPROPERTY(EditAnywhere, Category= "Movement",  meta = (AllowPrivateAccess = true))
	float AcceptanceRadius = 0;

	UPROPERTY(EditAnywhere, Category= "Movement",  meta = (AllowPrivateAccess = true))
	float RetreatDistance = 0;

	UPROPERTY(EditAnywhere, Category= "Movement",  meta = (AllowPrivateAccess = true))
	float StuckDistanceThreshold = 40.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float StuckCheckInterval = 1.0f;

	float LastLocationCheckTime = 0.0f;

	//boolean values
	bool bWasStuckLastCheck = false;

	bool bShouldPatrol = false;

	UPROPERTY(EditAnywhere,  Category= "Movement", meta = (AllowPrivateAccess = true))
	bool bShouldAlwaysChaseTarget = false;

	//FVector values
	FVector LastLocation = FVector::ZeroVector;

	FVector AttackTargetLocation = FVector::ZeroVector;

	FVector AttackTargetLocationInAir = FVector::ZeroVector;

	//Actor references
	UPROPERTY(EditAnywhere, Category = "Combat", meta = (AllowPrivateAccess = true))
	AActor* AttackTarget;
	
};
