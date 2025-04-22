#include "SGUtilObjectPoolManager.h"

ASGUtilObjectPoolManager::ASGUtilObjectPoolManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ASGUtilObjectPoolManager::InitializePool(const TSubclassOf<AActor>& ObjectClass, const int32 PoolSize)
{	
	if (Pools.Contains(ObjectClass))
	{
		return;
	}
	
	TArray<AActor*> NewPool;
	AddActorsToPool(ObjectClass, PoolSize, NewPool);

	const FActorPool NewPoolStruct(NewPool);
	Pools.Add(ObjectClass, NewPoolStruct);	
}

AActor* ASGUtilObjectPoolManager::GetPooledObject(TSubclassOf<AActor>& ObjectClass)
{
	if (!Pools.Contains(ObjectClass))
	{		
		InitializePool(ObjectClass, 5);
		return GetPooledObject(ObjectClass);
	}

	FActorPool Pool = Pools[ObjectClass];
	for (AActor* Object : Pool.Actors)
	{
		if (!Object->IsActorTickEnabled()) 
		{
			Object->SetActorEnableCollision(true);
			Object->SetActorHiddenInGame(false);
			Object->SetActorTickEnabled(true);
			return Object;
		}
	}
	ExpandPool(ObjectClass, 5);
	return GetPooledObject(ObjectClass);
}

void ASGUtilObjectPoolManager::ReturnObjectToPool(AActor* Object)
{
	if (!Object || !Pools.Contains(Object->GetClass()))
	{
		return;
	}
	
	Object->SetActorEnableCollision(false);
	Object->SetActorHiddenInGame(true);
	Object->SetActorTickEnabled(false);	
}

void ASGUtilObjectPoolManager::ExpandPool(const TSubclassOf<AActor>& ObjectClass, const int32 AdditionalSize)
{
	if (!Pools.Contains(ObjectClass))
	{
		return;
	}

	FActorPool& Pool = Pools[ObjectClass];
	AddActorsToPool(ObjectClass, AdditionalSize, Pool.Actors);	
}

void ASGUtilObjectPoolManager::AddActorsToPool(const TSubclassOf<AActor>& ObjectClass, const int32 Size, TArray<AActor*>& Actors) const
{
	for (int i = 0; i < Size; ++i)
	{
		AActor* NewObject = GetWorld()->SpawnActor<AActor>(ObjectClass);
		if (NewObject)
		{			
			NewObject->SetActorEnableCollision(false);
			NewObject->SetActorHiddenInGame(true);
			NewObject->SetActorTickEnabled(false);
			Actors.Add(NewObject);
		}
	}
}