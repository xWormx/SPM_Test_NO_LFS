/*

Written by
----------------------
- Carl-Johan Larson Eliasson
- cael7567
	
*/

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
