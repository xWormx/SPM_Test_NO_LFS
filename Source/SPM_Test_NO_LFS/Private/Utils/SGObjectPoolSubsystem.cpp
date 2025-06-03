#include "Utils/SGObjectPoolSubsystem.h"
#include "SPM_Test_NO_LFS.h"

void USGObjectPoolSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &USGObjectPoolSubsystem::OnPreLevelChange);
}

void USGObjectPoolSubsystem::Deinitialize()
{
	FCoreUObjectDelegates::PreLoadMap.RemoveAll(this);
	CleanupAllPools();

	Super::Deinitialize();
}

void USGObjectPoolSubsystem::InitializePool(const TSubclassOf<AActor>& ObjectClass, const int32 PoolSize, const bool bPersistAcrossLevels)
{
	if (Pools.Contains(ObjectClass))
	{
		return;
	}

	FActorPool NewPoolStruct;
	AddActorsToPool(ObjectClass, PoolSize, NewPoolStruct);
	Pools.Add(ObjectClass, NewPoolStruct);
}

AActor* USGObjectPoolSubsystem::GetPooledObject(TSubclassOf<AActor>& ObjectClass)
{
	if (!Pools.Contains(ObjectClass))
	{
		InitializePool(ObjectClass, InitialSize);
		return GetPooledObject(ObjectClass);
	}

	FActorPool& Pool = Pools[ObjectClass];

	for (int32 i = Pool.Actors.Num() - 1; i >= 0; i--)
	{
		if (!Pool.Actors[i] || Pool.Actors[i]->IsPendingKillPending())
		{
			Pool.Actors.RemoveAt(i);
		}
	}

	for (AActor* Object : Pool.Actors)
	{
		if (Object && IsValid(Object) && !Object->IsActorTickEnabled())
		{
			Pool.ActorTemplate.ApplyToActorRuntime(Object, true);
			Object->SetActorLocation(FVector(0, 0, 0));

			return Object;
		}
	}

	ExpandPool(ObjectClass, FMath::Max(1, Pool.DefaultPoolGrowthSize));
	return GetPooledObject(ObjectClass);
}

void USGObjectPoolSubsystem::ReturnObjectToPool(AActor* Object) 
{
	if (!Object)
	{
		return;
	}
	if(!Pools.Contains(Object->GetClass()))
	{
		InitializePool(Object->GetClass(), InitialSize);
	}
	else if (!Pools[Object->GetClass()].Actors.Contains(Object))
	{
		Pools[Object->GetClass()].Actors.Add(Object);
	}
	FActorPool& Pool = Pools[Object->GetClass()];
	Pool.ActorTemplate.ApplyToActorRuntime(Object, false);
	Object->SetActorLocation(Pool.PoolLocation); // Quick fix
}

int32 USGObjectPoolSubsystem::GetPoolSize(const TSubclassOf<AActor>& ObjectClass) const
{
	return Pools.Contains(ObjectClass) ? Pools[ObjectClass].Actors.Num() : 0;
}

void USGObjectPoolSubsystem::OnPreLevelChange([[maybe_unused]] const FString& MapName)
{
	for (auto It = Pools.CreateIterator(); It; ++It)
	{
		if (!It.Value().bPersistAcrossLevels)
		{
			for (AActor* Actor : It.Value().Actors)
			{
				if (Actor)
				{
					Actor->Destroy();
				}
			}
			It.RemoveCurrent();
		}
	}
}

void USGObjectPoolSubsystem::CleanupAllPools()
{
	for (auto It = Pools.CreateIterator(); It; ++It)
	{
		for (AActor* Actor : It.Value().Actors)
		{
			if (Actor)
			{
				Actor->Destroy();
			}
		}
		It.RemoveCurrent();
	}
}

void USGObjectPoolSubsystem::ExpandPool(const TSubclassOf<AActor>& ObjectClass, int32 AdditionalSize)
{
	if (!Pools.Contains(ObjectClass))
	{
		return;
	}

	FActorPool& Pool = Pools[ObjectClass];
	AddActorsToPool(ObjectClass, AdditionalSize, Pool);
}

void USGObjectPoolSubsystem::AddActorsToPool(const TSubclassOf<AActor>& ObjectClass, const int32 Size, FActorPool& Pool)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	// För att undvika collision vid spawn
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(Pool.PoolLocation);

	for (int i = 0; i < Size; ++i)
	{
		const double OffsetValue = 100*i;
		const FVector Offset = FVector(OffsetValue, OffsetValue, 0);
		const FVector SpawnLocation = SpawnTransform.GetLocation() + Offset;
		SpawnTransform.SetLocation(SpawnLocation);

		AActor* NewObject = World->SpawnActor<AActor>(ObjectClass, SpawnTransform, Pool.SpawnParams);
		if (!NewObject || !IsValid(NewObject))
		{
			EMMA_LOG(Error, TEXT("Failed to spawn %s for object pool."), *ObjectClass->GetName());
			continue; // Skip if spawn failed
		}
		if (i == 0 && Pool.Actors.IsEmpty())
		{
			Pool.ActorTemplate.MakeTemplateOfActorRuntime(NewObject);
		}
		Pool.ActorTemplate.ApplyToActorRuntime(NewObject, false);
		Pool.Actors.Add(NewObject);
	}
}

void USGObjectPoolSubsystem::PerformPoolMaintenance()
{
	for (TPair<TSubclassOf<AActor>, FActorPool>& PoolPair : Pools)
	{
		FActorPool& Pool = PoolPair.Value;

		for (int32 i = Pool.Actors.Num() - 1; i >= 0; i--)
		{
			AActor* Actor = Pool.Actors[i];
			if (!Actor || !IsValid(Actor) || Actor->IsPendingKillPending())
			{
				EMMA_LOG(Warning, TEXT("Removed invalid actor from pool for class %s"), *PoolPair.Key->GetName());
				Pool.Actors.RemoveAt(i);
			}
		}
	}
}
