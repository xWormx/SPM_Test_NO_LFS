

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


};
