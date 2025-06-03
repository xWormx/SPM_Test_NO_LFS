

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

protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Air Movement", meta = (AllowPrivateAccess = "true"))
	float HoverAmplitude = 50.f;

	UPROPERTY(EditAnywhere, Category = "Air Movement", meta = (AllowPrivateAccess = "true"))
	float HoverSpeed = 1.5;

	UPROPERTY(EditAnywhere, Category = "Air Movement", meta = (AllowPrivateAccess = "true"))
	float HoverInterpSpeed = 0.5;


};
