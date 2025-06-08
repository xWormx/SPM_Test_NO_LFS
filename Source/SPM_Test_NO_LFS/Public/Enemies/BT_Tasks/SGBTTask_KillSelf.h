// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SGBTTask_KillSelf.generated.h"


UCLASS()
class SPM_TEST_NO_LFS_API USGBTTask_KillSelf : public UBTTaskNode
{
	GENERATED_BODY()

public:
	USGBTTask_KillSelf();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
