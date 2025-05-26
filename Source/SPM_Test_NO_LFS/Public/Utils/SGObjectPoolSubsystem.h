#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SGObjectPoolSubsystem.generated.h"

USTRUCT(BlueprintType, Category = "ActorPool")
struct FActorPool
{
	GENERATED_BODY()

	FActorPool() {}
	explicit FActorPool(const TArray<AActor*>& InActors) : Actors(InActors) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Pooled Actors", meta = (AllowPrivateAccess = "true"))
	int DefaultPoolGrowthSize = 5;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UProperty - Pooled Actors")
	TArray<AActor*> Actors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Pooled Actors")
	bool bPersistAcrossLevels = true;
};

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
	void AddActorsToPool(const TSubclassOf<AActor>& ObjectClass, int32 Size, TArray<AActor*>& Actors);

	void PerformPoolMaintenance();

	UPROPERTY()
	TMap<TSubclassOf<AActor>, FActorPool> Pools;

	UPROPERTY()
	int InitialSize = 5;

	FTimerHandle PoolMaintenanceTimer;
};
