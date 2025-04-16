#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGEnemySpawnPoint.generated.h"

class ASGEnemyCharacter;

UCLASS()
class SPM_TEST_NO_LFS_API ASGEnemySpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	ASGEnemySpawnPoint();
	virtual void Tick(float DeltaTime) override;
	ASGEnemyCharacter* SpawnEnemy(const TSubclassOf<ASGEnemyCharacter> EnemyClass) const;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="design",meta=(AllowPrivateAccess="true"))
	USoundBase* SpawnSound;
};
