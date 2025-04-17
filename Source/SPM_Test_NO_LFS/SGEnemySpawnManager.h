#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGEnemySpawnManager.generated.h"

class ASGEnemySpawnPoint;
class ASGEnemyCharacter;

UCLASS()
class SPM_TEST_NO_LFS_API ASGEnemySpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ASGEnemySpawnManager();
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	void HandleEnemyDeath(ASGEnemyCharacter* DeadEnemy);

protected:
	virtual void BeginPlay() override;

private:
	void StartIntermissionTimer();
	void EndIntermissionTimer();
	void StartNextWave();
	void SpawnEnemies();
	const ASGEnemySpawnPoint* GetRandomSpawnPoint() const;
	const TSubclassOf<ASGEnemyCharacter> GetRandomEnemyType() const;
	
	TArray<AActor*> EnemySpawnPoints;
	int32 CurrentWave = 0;
	int32 EnemiesAlive = 0;
	FTimerHandle IntermissionTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	float TimeBetweenWaves = 9.9f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	float EnemyCountScalingFactor = 1.75f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="design",meta=(AllowPrivateAccess="true"))
	USoundBase* SpawnSound;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="design",meta=(AllowPrivateAccess="true"))
	TArray<TSubclassOf<ASGEnemyCharacter>> EnemyTypes;
};
