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

	UE_LOG(LogTemp, Warning, TEXT("Initializing pool for class: %s"), *ObjectClass->GetName());

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
	if (!Object)
	{
		return;
	}

	const TSubclassOf<AActor> ObjectClass = Object->GetClass();
	if (!Pools.Contains(ObjectClass))
	{
		return;
	}
	
	Object->SetActorEnableCollision(false);
	Object->SetActorHiddenInGame(true);
	Object->SetActorTickEnabled(false);
	Pools[ObjectClass].Actors.Add(Object);	
}

void ASGUtilObjectPoolManager::ExpandPool(const TSubclassOf<AActor>& ObjectClass, const int32 AdditionalSize)
{
	if (!Pools.Contains(ObjectClass))
	{
		return;
	}

	FActorPool& Pool = Pools[ObjectClass];
	UE_LOG(LogTemp, Error, TEXT("Expanding pool for class: %s to %d"), *ObjectClass->GetName(), Pool.Actors.Num()+AdditionalSize);
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