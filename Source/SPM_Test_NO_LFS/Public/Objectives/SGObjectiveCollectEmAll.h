// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGObjectiveBase.h"
#include "SGObjectiveCollectEmAll.generated.h"

/**
 * 
 */
UCLASS()
class SPM_TEST_NO_LFS_API ASGObjectiveCollectEmAll : public ASGObjectiveBase
{
	GENERATED_BODY()
public:
	virtual void OnStart(ASGGameObjectivesHandler* ObjectiveHandler) override;
	virtual bool IsCompleted(ASGGameObjectivesHandler* ObjectiveHandler) override;
	virtual void OnCompleted(ASGGameObjectivesHandler* ObjectiveHandler) override;
	virtual void Update(ASGGameObjectivesHandler* ObjectiveHandler) override;
	virtual EObjectiveType GetObjectiveType() override { return EObjectiveType::EOT_Collect; }
private:
	UPROPERTY(EditAnywhere, Category=UPROPERTY)
	unsigned int CollectGoal;

	UPROPERTY(EditAnywhere ,Category=UPROPERTY)
	unsigned int CurrentCollected = 0;

	/*
	 TArray<UPickups> PickUps
	 */
};

