#pragma once
#include "CoreMinimal.h"
#include "SGActorTemplate.h"
#include "SGActorPool.generated.h"

USTRUCT(BlueprintType, Category = "ActorPool")
struct FActorPool
{
	GENERATED_BODY()

	FActorPool()
	{
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	}
	FActorPool(const TArray<AActor*>& InActors) : Actors(InActors) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Pooled Actors", meta = (AllowPrivateAccess = "true"))
	int DefaultPoolGrowthSize = 5;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UProperty - Pooled Actors")
	TArray<AActor*> Actors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Pooled Actors")
	bool bPersistAcrossLevels = true;

	FVector PoolLocation = FVector(0, 0, -10000); // Default location to avoid collisions at spawn
	FActorSpawnParameters SpawnParams;
	FActorTemplate ActorTemplate;
};
