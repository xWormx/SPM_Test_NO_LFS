// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CJGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SPM_TEST_NO_LFS_API ACJGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	
};
