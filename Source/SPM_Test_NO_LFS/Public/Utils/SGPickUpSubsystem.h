#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SGPickUpSubsystem.generated.h"

class USGObjectPoolSubsystem;
class ASGEnemyCharacter;
struct FEnemyDropInfo;

UCLASS()
class SPM_TEST_NO_LFS_API USGPickUpSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public:
	void DropItem(ASGEnemyCharacter* EnemyCharacter) const;

private:
	TMap<TSubclassOf<ASGEnemyCharacter>, TArray<FEnemyDropInfo>> EnemyDropMap;

};
