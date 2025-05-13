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

USTRUCT()
struct FDespawnCandidate
{
	GENERATED_BODY()
	ASGEnemyCharacter* Enemy;
	float TimeOutOfRange;
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
	void SetMaxEnemiesAtATime(int32 NewMaxEnemiesAtATime);
	
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
	void CheckIfDespawnCandidate(ASGEnemyCharacter* Enemy);
	
	UFUNCTION()
	void HandleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void HandleEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void HandleDefendEventStart();
	UFUNCTION()
	void HandleDefendEventEnd(UObject* ObjectiveInterfaceImplementor);

	bool bSpawningIsActive = false;
	TArray<AActor*> AllEnemySpawnPoints;
	ASGPlayerCharacter* PlayerCharacter;
	int32 EnemiesAlive = 0;
	FTimerHandle SpawnLoopTimer;
	FTimerHandle DespawnTimer;
	uint32 SpawnAreaIndex = 0;
	uint32 EnemyCount = 1;
	ESpawnMode DefaultSpawnMode;
	int32 DefaultMaxEnemiesAlive;
	float DefaultTimeBetweenSpawns;

	UPROPERTY()
	TArray<FDespawnCandidate> DespawnCandidates;
	UPROPERTY(VisibleAnywhere, Category="design")
	class USphereComponent* DespawnTriggerSphere;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	float DespawnTriggerRadius = 5000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	float DespawnGracePeriod = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	int32 MaxEnemiesAlive = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	int32 MaxEnemiesAliveDefendThePodMultiplier = 5;
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	class ASGObjectiveDefendThePod* ObjectiveDefendThePod;
};