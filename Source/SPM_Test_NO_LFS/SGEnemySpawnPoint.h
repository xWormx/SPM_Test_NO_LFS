#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGEnemySpawnPoint.generated.h"

UCLASS()
class SPM_TEST_NO_LFS_API ASGEnemySpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	ASGEnemySpawnPoint();
	virtual void Tick(float DeltaTime) override;
	void SpawnEnemy() const;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	TSubclassOf<AActor> EnemyClass;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="design",meta=(AllowPrivateAccess="true"))
	USoundBase* SpawnSound;
};
