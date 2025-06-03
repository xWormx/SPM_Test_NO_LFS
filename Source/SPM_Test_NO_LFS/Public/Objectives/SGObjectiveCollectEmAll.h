/*

Written by
----------------------
- Carl-Johan Larson Eliasson
- cael7567
	
*/
#pragma once

#include "CoreMinimal.h"
#include "SGObjectiveBase.h"
#include "SGObjectiveCollectEmAll.generated.h"

class ASGPickUpObjectiveCollect;

UCLASS()
class SPM_TEST_NO_LFS_API ASGObjectiveCollectEmAll : public ASGObjectiveBase
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
	void SetNumberToCollect(int InNumToCollect) { CollectGoal = InNumToCollect; }
	virtual EObjectiveType GetObjectiveType() override { return EObjectiveType::EOT_Collect; }
private:
	UPROPERTY(VisibleAnywhere, Category=UPROPERTY)
	unsigned int CollectGoal;

	UPROPERTY(VisibleAnywhere ,Category=UPROPERTY)
	unsigned int CurrentCollected = 0;

public:
	UPROPERTY(VisibleAnywhere, Category = "UPROPERTY")
	TSubclassOf<ASGPickUpObjectiveCollect> CollectableClass;

	// För att gömma dem när Objectives Laddas genom LoadGame
	void HideCollectables() const;
private:
	void ShowCollectables() const;
	
};

