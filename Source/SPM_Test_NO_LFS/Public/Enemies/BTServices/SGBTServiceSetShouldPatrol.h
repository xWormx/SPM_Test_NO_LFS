// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGBTServiceBase.h"
#include "SGBTServiceSetShouldPatrol.generated.h"

/**
 * 
 */
UCLASS()
class SPM_TEST_NO_LFS_API USGBTServiceSetShouldPatrol : public USGBTServiceBase
{
	GENERATED_BODY()

public:
	USGBTServiceSetShouldPatrol();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual void PatrolDelay();

	bool bShouldPatrol = false;
};
