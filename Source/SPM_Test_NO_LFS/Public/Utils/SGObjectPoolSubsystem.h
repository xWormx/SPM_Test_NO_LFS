#pragma once

#include "CoreMinimal.h"
#include "ObjectPoolData/SGActorPool.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SGObjectPoolSubsystem.generated.h"

UCLASS()
class SPM_TEST_NO_LFS_API USGObjectPoolSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	void InitializePool(const TSubclassOf<AActor>& ObjectClass, int32 PoolSize, bool bPersistAcrossLevels = true);

	AActor* GetPooledObject(TSubclassOf<AActor>& ObjectClass);
	void ReturnObjectToPool(AActor* Object);

	int32 GetPoolSize(const TSubclassOf<AActor>& ObjectClass) const;
private:
	UFUNCTION()
	void OnPreLevelChange(const FString& MapName);
	void CleanupAllPools();

	void ExpandPool(const TSubclassOf<AActor>& ObjectClass, int32 AdditionalSize);
	void AddActorsToPool(const TSubclassOf<AActor>& ObjectClass, int32 Size, FActorPool& Pool);

	void PerformPoolMaintenance();

	UPROPERTY()
	TMap<TSubclassOf<AActor>, FActorPool> Pools;

	UPROPERTY()
	int InitialSize = 5;

	FTimerHandle PoolMaintenanceTimer;
};
