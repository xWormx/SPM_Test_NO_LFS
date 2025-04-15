// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "SGBTServie_PlayerLocation.generated.h"

/**
 * 
 */
UCLASS()
class SPM_TEST_NO_LFS_API USGBTServie_PlayerLocation : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	USGBTServie_PlayerLocation();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
