#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGEnemySpawnManager.generated.h"

UCLASS()
class SPM_TEST_NO_LFS_API ASGEnemySpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ASGEnemySpawnManager();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void StartNextWave();

protected:
	virtual void BeginPlay() override;

private:
	void StartIntermissionTimer();
	void EndIntermissionTimer();
	void SpawnEnemies();
	
	TArray<AActor*> EnemySpawnPoints;
	int32 CurrentWave = 0;
	FTimerHandle IntermissionTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	float TimeBetweenWaves = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	int EnemyCountScalingFactor = 2;
};
