#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGEnemySpawnManager.generated.h"

class ASGEnemySpawnPoint;
class ASGEnemyCharacter;
class ASGPlayerCharacter;

USTRUCT(BlueprintType)
struct FSpawnPointGroup
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design")
	TArray<AActor*> EnemySpawnPoints;
};

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
	void SpawnEnemiesEverywhere();
	void SpawnEnemiesFromGroup(uint32 GroupNumber);
	const ASGEnemySpawnPoint* GetRandomSpawnPoint(TArray<AActor*> SpawnPointArray) const;
	const TSubclassOf<ASGEnemyCharacter> GetRandomEnemyType() const;
	
	TArray<AActor*> AllEnemySpawnPoints;
	ASGPlayerCharacter* PlayerCharacter;
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
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="design",meta=(AllowPrivateAccess="true"))
	float MinDistanceFromPlayer = 3000.0f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="design",meta=(AllowPrivateAccess="true"))
	TArray<FSpawnPointGroup> EnemySpawnPointGroups;
};
