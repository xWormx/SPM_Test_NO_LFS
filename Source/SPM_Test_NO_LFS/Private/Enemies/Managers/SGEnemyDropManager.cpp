#include "Enemies/Managers/SGEnemyDropManager.h"

#include "Enemies/Characters/SGEnemyCharacter.h"
#include "Pickups/SGPickUp.h"
#include "Utils/SGUtilObjectPoolManager.h"
#include "Kismet/GameplayStatics.h"

ASGEnemyDropManager::ASGEnemyDropManager()
{
	PrimaryActorTick.bCanEverTick = false;
}


void ASGEnemyDropManager::BeginPlay()
{
	Super::BeginPlay();

	ObjectPoolManager = Cast<ASGUtilObjectPoolManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ASGUtilObjectPoolManager::StaticClass()));
	if (!ObjectPoolManager || !EnemyDropDataTable)
	{
		return;
	}

	auto PreloadEnemyDropData = [&](const FName&, const FEnemyDropInfo& Drop)
	{
		if (!Drop.EnemyClass)
		{
			return;
		}
		TSubclassOf<AActor> DropClass = TSubclassOf(Drop.PickUpClass);
		ObjectPoolManager->InitializePool(DropClass, Drop.PickUpCount);
	};
	EnemyDropDataTable->ForeachRow<FEnemyDropInfo>(TEXT("Initialize Enemy Drop Data"),PreloadEnemyDropData);
}

void ASGEnemyDropManager::DropItem(ASGEnemyCharacter* EnemyCharacter) const
{
	if (!EnemyCharacter || !EnemyDropDataTable || !ObjectPoolManager)
	{
		return;
	}

	const FVector SpawnLocation = EnemyCharacter->GetActorLocation();
	const FRotator SpawnRotation = FRotator::ZeroRotator;

	const FString DebugMessage = TEXT("Iterate Enemy Drop Data");
	auto DropLoot = [&]([[maybe_unused]] const FName& Key, const FEnemyDropInfo& Drop)
	{
		if (!EnemyCharacter->GetClass()->IsChildOf(Drop.EnemyClass))
		{
			return;
		}
		for (int32 i = 0; i < Drop.PickUpCount; ++i)
		{
			TSubclassOf<AActor> DropClass = TSubclassOf(Drop.PickUpClass);
			if (AActor* PooledObject = ObjectPoolManager->GetPooledObject(DropClass))
			{
				PooledObject->SetActorLocation(SpawnLocation);
				PooledObject->SetActorRotation(SpawnRotation);
				PooledObject->SetActorHiddenInGame(false);
				PooledObject->SetActorEnableCollision(true);
			}		
		}
	};

	EnemyDropDataTable->ForeachRow<FEnemyDropInfo>(DebugMessage, DropLoot);
}
