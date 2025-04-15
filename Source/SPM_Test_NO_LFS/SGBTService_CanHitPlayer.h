// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "SGBTService_CanHitPlayer.generated.h"

/**
 * 
 */
UCLASS()
class SPM_TEST_NO_LFS_API USGBTService_CanHitPlayer : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	USGBTService_CanHitPlayer();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	float HitRange = 100.f;
	
};
