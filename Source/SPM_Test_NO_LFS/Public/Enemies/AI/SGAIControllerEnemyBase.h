// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SGAIControllerEnemyBase.generated.h"

class ASGEnemyCharacter;
/**
 * 
 */
UCLASS()
class SPM_TEST_NO_LFS_API ASGAIControllerEnemyBase : public AAIController
{
	GENERATED_BODY()
protected:
	ASGAIControllerEnemyBase();
	
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	virtual bool CanAttackTarget() const;
	
	virtual void HandleMovement();

	UFUNCTION(BlueprintCallable)
	virtual float GetAttackRange() const;

	UFUNCTION(BlueprintCallable)
	virtual void SetAttackRange(const float NewAttackRange);

	UFUNCTION(BlueprintCallable)
	virtual AActor* GetAttackTarget() const;
	
	UFUNCTION(BlueprintCallable)
	virtual void SetAttackTarget(AActor* NewAttackTarget);

	UFUNCTION(BlueprintCallable)
	virtual void SetControlledCharacter(ASGEnemyCharacter* NewControlledEnemy);
	
	UFUNCTION(BlueprintCallable)
	virtual float GetAcceptanceRadius() const;

	UFUNCTION(BlueprintCallable)
	virtual void SetAcceptanceRadius(const float NewAcceptanceRadius);

	UFUNCTION(BlueprintCallable)
	virtual float GetRetreatDistance() const;

	UFUNCTION(BlueprintCallable)
	virtual void SetRetreatDistance(const float NewRetreatDistance);

	UPROPERTY(EditAnywhere, Category= "Combat", meta = (AllowPrivateAccess = true))
	float AttackRange = 0;

	UPROPERTY(EditAnywhere,  Category= "Combat", meta = (AllowPrivateAccess = true))
	TObjectPtr<AActor> AttackTarget;
	
	TObjectPtr<ASGEnemyCharacter> ControlledEnemy;

	UPROPERTY(EditAnywhere, Category= "Movement",  meta = (AllowPrivateAccess = true))
	float AcceptanceRadius = 0;

	UPROPERTY(EditAnywhere,  Category= "Movement", meta = (AllowPrivateAccess = true))
	float RetreatDistance = 0;

	UPROPERTY(EditAnywhere,  Category= "Movement", meta = (AllowPrivateAccess = true))
	bool bShouldAlwaysChaseTarget = false;

	bool bIsAttacking = false;

	virtual bool IsFacingTarget() const;

	virtual void RotateTowardsTargetWhileNotMoving();

public:
	virtual void Tick(float DeltaTime) override;
	
};
