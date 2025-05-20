// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGObjectiveBase.h"
#include "SGObjectiveKillEmAll.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SPM_TEST_NO_LFS_API ASGObjectiveKillEmAll : public ASGObjectiveBase
{
	GENERATED_BODY()
public:
	/*
	virtual void OnStart(ASGGameObjectivesHandler* ObjectiveHandler) override;
	virtual bool IsCompleted(ASGGameObjectivesHandler* ObjectiveHandler) override;
	virtual void OnCompleted(ASGGameObjectivesHandler* ObjectiveHandler) override;
	virtual void Update(ASGGameObjectivesHandler* ObjectiveHandler) override;
	*/
	virtual bool IsCompleted() override;
	virtual void OnStart() override;
	virtual void OnCompleted() override;
	virtual void Update() override;
	virtual EObjectiveType GetObjectiveType() override { return EObjectiveType::EOT_KillAllEnemies; }
	void SetEnemiesToKill(int InEnemiesToKill) { EnemiesToKill = InEnemiesToKill; }
private:
	UPROPERTY(VisibleAnywhere, Category=UPROPERTY)
	unsigned int EnemiesToKill;

	UPROPERTY(VisibleAnywhere ,Category=UPROPERTY)
	unsigned int CurrentEnemiesKilled = 0;
};
