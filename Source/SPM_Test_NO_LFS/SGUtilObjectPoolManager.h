#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGUtilObjectPoolManager.generated.h"

class ISGUtilObjectPooledInterface;

USTRUCT(BlueprintType, Category = "ActorPool")
struct FActorPool
{
	GENERATED_BODY()
	
	FActorPool() {}
	explicit FActorPool(const TArray<AActor*>& InActors) : Actors(InActors) {}

	//TODO: Lägga till X antal att skapa? Om man vill sköta det i editor?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Pooled Actors", meta = (AllowPrivateAccess = "true"))
	int DefaultPoolGrowthSize = 5;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UProperty - Pooled Actors")
	TArray<AActor*> Actors;	
};

UCLASS()
class SPM_TEST_NO_LFS_API ASGUtilObjectPoolManager : public AActor
{
	GENERATED_BODY()
	
//TODO: Överlagra BeginPlay() för att själva initiera pooler som lagts till via editorn? Just nu måste all initiering ske från andra klasser (se SGEnemyDropManager.cpp)

public:
	ASGUtilObjectPoolManager();
	void InitializePool(const TSubclassOf<AActor>& ObjectClass, int32 PoolSize);
	AActor* GetPooledObject(TSubclassOf<AActor>& ObjectClass);
	void ReturnObjectToPool(AActor* Object);

private:
	void ExpandPool(const TSubclassOf<AActor>& ObjectClass, int32 AdditionalSize);
	void AddActorsToPool(const TSubclassOf<AActor>& ObjectClass, const int32 Size, TArray<AActor*>& Actors) const;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UProperty - Actor Pool", meta = (AllowPrivateAccess = "true"))
	TMap<TSubclassOf<AActor>, FActorPool> Pools;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UProperty - Actor Pool", meta = (AllowPrivateAccess = "true"))
	int InitialSize = 5;
};