#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGEnemySpawnManager.generated.h"

class ASGEnemySpawnPoint;
class ASGEnemyCharacter;
class ASGPlayerCharacter;

UENUM(BlueprintType)
enum class ESpawnMode : uint8
{
	Everywhere, AtArea, AroundPlayer
};

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
	void StartSpawning();
	void StopSpawning();
	void SetSpawnMode(ESpawnMode NewMode);
	void SetSpawnArea(uint32 Index);
	void SetEnemyCount(uint32 NewEnemyCount);
	
	UFUNCTION()
	void HandleEnemyDeath(ASGEnemyCharacter* DeadEnemy);

protected:
	virtual void BeginPlay() override;

private:
	void StartSpawnLoopTimer();
	void EndSpawnLoopTimer();
	void SpawnEnemiesEverywhere();
	void SpawnEnemiesAtArea();
	void SpawnEnemiesAroundPlayer();
	void SpawnEnemies(const TArray<AActor*>& AvailableSpawnPoints);
	const ASGEnemySpawnPoint* GetRandomSpawnPoint(TArray<AActor*> SpawnPointArray) const;
	const TSubclassOf<ASGEnemyCharacter> GetRandomEnemyType() const;

	bool bSpawningIsActive = false;
	TArray<AActor*> AllEnemySpawnPoints;
	ASGPlayerCharacter* PlayerCharacter;
	uint32 EnemiesAlive = 0;
	FTimerHandle SpawnLoopTimer;
	uint32 SpawnAreaIndex = 0;
	uint32 EnemyCount = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	float TimeBetweenSpawns = 1.0f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="design",meta=(AllowPrivateAccess="true"))
	USoundBase* SpawnSound;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="design",meta=(AllowPrivateAccess="true"))
	TArray<TSubclassOf<ASGEnemyCharacter>> EnemyTypes;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="design",meta=(AllowPrivateAccess="true"))
	float MinDistanceFromPlayer = 3000.0f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="design",meta=(AllowPrivateAccess="true"))
	float SpawnRadiusAroundPlayer = 5500.0f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="design",meta=(AllowPrivateAccess="true"))
	TArray<FSpawnPointGroup> EnemySpawnPointGroups;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="design",meta=(AllowPrivateAccess="true"))
	ESpawnMode SpawnMode = ESpawnMode::Everywhere;
};
