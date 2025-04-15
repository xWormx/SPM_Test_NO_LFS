// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "SGBTTask_ClearBlackBoardValue.generated.h"

/**
 * 
 */
UCLASS()
class SPM_TEST_NO_LFS_API USGBTTask_ClearBlackBoardValue : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	USGBTTask_ClearBlackBoardValue();
	
};
