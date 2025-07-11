// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGAIControllerEnemyBase.h"
#include "SGAIControllerEnemyFlying.generated.h"

class ASGEnemyMoveToPoint;
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

	virtual void SetFlyingMode(bool bShouldFly);

	virtual void HandleMovement() override;

	virtual void FlyTowardsTarget();

	virtual void FlyTowardsLocation(const FVector& TargetLocation);

	ASGEnemyMoveToPoint* GetClosestMoveToPoint();

	ASGEnemyMoveToPoint* GetRandomMoveToPoint() const;

	FVector GetClosestMoveToPointLocation();

	bool HasReachedCurrentMoveToPoint(float Tolerance) const;

	virtual void SetChaseAndAttackMode();

	virtual void SearchForTarget();

	FVector GetFallbackChaseLocation() const;

	virtual void UpdateMoveToPoints();
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

	UPROPERTY(EditAnywhere, Category = "Movement")
	TArray<AActor*> MoveToPoints;

	UPROPERTY(EditAnywhere, Category = "Movement")
	ASGEnemyMoveToPoint* CurrentMoveToPoint = nullptr;

	bool bHasFoundTarget = true;

	struct FTimerHandle HoverZTimerHandle;

	FVector CurrentLocation;
};
