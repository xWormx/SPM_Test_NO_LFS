#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SGAIControllerEnemy.generated.h"

class ASGEnemySpawnManager;
class ASGEnemyCharacter;

UCLASS()
class SPM_TEST_NO_LFS_API ASGAIControllerEnemy : public AAIController
{
	GENERATED_BODY()

public:
	
	ASGAIControllerEnemy();

	virtual void Tick(float DeltaTime) override;

	virtual void Patrol();

	virtual void PatrolDelay();

	virtual void RotateTowardsTarget();

	virtual void SetBehaviorTreeEnabled(bool bEnabled);

	virtual void ClearBlackboardValues();

	//FVector helpers
	virtual FVector GetFallbackChaseLocation() const;
	
	//float helpers
	UFUNCTION(BlueprintCallable)
	virtual float GetCharacterVelocity() const;

	//boolean helpers
	virtual bool CanReachTarget();

	UFUNCTION(BlueprintCallable)
	virtual bool CanAttackTarget();

	UFUNCTION(BlueprintCallable)
	virtual bool IsFacingTarget() const;

	virtual bool IsStuck();

	virtual bool IsStuckOutsideNavMesh();

	virtual bool HasReachedPatrolPoint(float Tolerance);

	virtual void UpdatePatrolPoints();

	virtual bool CanReachTarget(AActor* Target);

	//Getters
	UFUNCTION(BlueprintCallable)
	virtual AActor* GetAttackTarget();

	virtual ASGEnemyCharacter* GetControlledEnemy();

	//Boolean values
	bool bShouldPatrol = false;

	//Float values
	UPROPERTY(EditAnywhere, Category= "Movement",  meta = (AllowPrivateAccess = true))
	float AcceptanceRadius = 0;

	UPROPERTY(EditAnywhere, Category= "Movement",  meta = (AllowPrivateAccess = true))
	float RetreatDistance = 0;

	//TimerHandles
	FTimerHandle PatrolDelayTimer;

	TArray<AActor*> SpawnManagers;

protected:
	
	virtual void BeginPlay() override;

	virtual void SetInitialValues();
	
	virtual class AActor* GetPatrolPoint();

	virtual void SetShouldPatrol();

	//Behavior Tree
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* EnemyBehaviorTree;
	
	//float values
	UPROPERTY(EditAnywhere, Category= "Combat",  meta = (AllowPrivateAccess = true))
	float AttackRange = 0;

	UPROPERTY(EditAnywhere, Category= "Movement",  meta = (AllowPrivateAccess = true))
	float StuckDistanceThreshold = 40.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float StuckCheckInterval = 1.0f;

	float LastLocationCheckTime = 0.0f;

	//Boolean values
	bool bWasStuckLastCheck = false;

	UPROPERTY(EditAnywhere,  Category= "Movement", meta = (AllowPrivateAccess = true))
	bool bShouldAlwaysChaseTarget = false;

	bool bShouldBeFlying = false;

	//FVector values
	FVector ControlledEnemyLocation = FVector::ZeroVector;
	
	FVector LastLocation = FVector::ZeroVector;

	FVector AttackTargetLocation = FVector::ZeroVector;

	FVector AttackTargetLocationInAir = FVector::ZeroVector;

	//Actor references
	UPROPERTY(EditAnywhere, Category = "Combat", meta = (AllowPrivateAccess = true))
	AActor* AttackTarget;

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = true))
	class ASGEnemyCharacter* ControlledEnemy;

	UPROPERTY(EditAnywhere, Category = "Movement")
	TArray<AActor*> PatrolPoints;

	UPROPERTY(EditAnywhere, Category = "Movement")
	AActor* CurrentPatrolPoint;
	
};
