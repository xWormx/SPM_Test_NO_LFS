// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGEnemyAttackComponentBase.h"
#include "SGEnemyChargeAttackComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SPM_TEST_NO_LFS_API USGEnemyChargeAttackComponent : public USGEnemyAttackComponentBase
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USGEnemyChargeAttackComponent();

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

	virtual void ChargeTowardsTarget(const FVector& TargetLocation);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
			   UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditAnywhere)
	float ChargeSpeed = 1500.f;

	bool bIsCharging = false;

	UPROPERTY()
	FVector RegisteredTargetLocation;

	UFUNCTION()
	void BackUpBeforeCharge();

	UFUNCTION()
	void FaceTarget(AActor* Target);

};
