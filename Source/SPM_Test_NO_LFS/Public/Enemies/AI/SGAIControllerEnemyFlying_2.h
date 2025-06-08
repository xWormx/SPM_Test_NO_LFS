

#pragma once

#include "CoreMinimal.h"
#include "SGAIControllerEnemy.h"
#include "SGAIControllerEnemyFlying_2.generated.h"

UCLASS()
class SPM_TEST_NO_LFS_API ASGAIControllerEnemyFlying_2 : public ASGAIControllerEnemy
{
	GENERATED_BODY()

public:
	ASGAIControllerEnemyFlying_2();

	virtual void Tick(float DeltaTime) override;

	virtual void SetFlyingMode(bool bShouldFly);

	virtual FVector GetFallbackChaseLocation() const override;

	virtual void FlyTowardsLocation(const FVector& TargetLocation);

	virtual void Patrol() override;
	
	//Not Currently Used
	virtual FVector PrepareAttack();

	virtual FVector& BackUpBeforeCharging();

	FVector RetreatTargetLocation = FVector::ZeroVector;

protected:
	
	virtual void BeginPlay() override;

	virtual void UpdateMoveToPoints();

	bool HasReachedCurrentMoveToPoint(float Tolerance) const;

	class ASGEnemyMoveToPoint* GetRandomMoveToPoint() const;

	UPROPERTY(EditAnywhere, Category = "Air Movement", meta = (AllowPrivateAccess = "true"))
	float HoverAmplitude = 50.f;

	UPROPERTY(EditAnywhere, Category = "Air Movement", meta = (AllowPrivateAccess = "true"))
	float HoverSpeed = 1.5;

	UPROPERTY(EditAnywhere, Category = "Air Movement", meta = (AllowPrivateAccess = "true"))
	float HoverInterpSpeed = 0.5;
	
	UPROPERTY(EditAnywhere, Category = "Movement")
	TArray<AActor*> MoveToPoints;

	UPROPERTY(EditAnywhere, Category = "Movement")
	class ASGEnemyMoveToPoint* CurrentMoveToPoint = nullptr;

	UPROPERTY(EditAnywhere, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float BackUpDistance = 150.f;

	UPROPERTY(EditAnywhere, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float BackUpSpeed = 5.f;

	
		
};
