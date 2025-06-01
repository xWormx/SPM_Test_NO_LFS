// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecoratorTest.generated.h"

/**
 * 
 */
UCLASS()
class SPM_TEST_NO_LFS_API UBTDecoratorTest : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTDecoratorTest();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
