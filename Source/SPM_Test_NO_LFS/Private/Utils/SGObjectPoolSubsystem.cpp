// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/SGObjectPoolSubsystem.h"

#include "Utils/SGUtilObjectPoolManager.h"

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

	TArray<AActor*> NewPool;
	AddActorsToPool(ObjectClass, PoolSize, NewPool);

	FActorPool NewPoolStruct(NewPool);
	NewPoolStruct.bPersistAcrossLevels = bPersistAcrossLevels;
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
			Object->SetActorEnableCollision(true);
			Object->SetActorHiddenInGame(false);
			Object->SetActorTickEnabled(true);

			Object->SetActorLocation(FVector(0, 0, 0));

			return Object;
		}
	}

	ExpandPool(ObjectClass, FMath::Max(1, Pool.DefaultPoolGrowthSize));
	return GetPooledObject(ObjectClass);
}

void USGObjectPoolSubsystem::ReturnObjectToPool(AActor* Object) const
{
	if (!Object || !Pools.Contains(Object->GetClass()))
	{
		return;
	}

	Object->SetActorEnableCollision(false);
	Object->SetActorHiddenInGame(true);
	Object->SetActorTickEnabled(false);
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
	AddActorsToPool(ObjectClass, AdditionalSize, Pool.Actors);
}

void USGObjectPoolSubsystem::AddActorsToPool(const TSubclassOf<AActor>& ObjectClass, const int32 Size, TArray<AActor*>& Actors)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// För att undvika collision vid spawn
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(FVector(0, 0, -10000));

	for (int i = 0; i < Size; ++i)
	{
		FVector Offset = FVector(100*i, 100*i, 0);
		FVector SpawnLocation = SpawnTransform.GetLocation() + Offset;
		SpawnTransform.SetLocation(SpawnLocation);

		AActor* NewObject = World->SpawnActor<AActor>(ObjectClass, SpawnTransform, SpawnParams);

		if (NewObject && IsValid(NewObject))
		{
			NewObject->SetActorEnableCollision(false);
			NewObject->SetActorHiddenInGame(true);
			NewObject->SetActorTickEnabled(false);
			Actors.Add(NewObject);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to spawn %s for object pool."), *ObjectClass->GetName());
		}
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
				UE_LOG(LogTemp, Warning, TEXT("Removed invalid actor from pool for class %s"), *PoolPair.Key->GetName());
				Pool.Actors.RemoveAt(i);
			}
		}
	}
}
