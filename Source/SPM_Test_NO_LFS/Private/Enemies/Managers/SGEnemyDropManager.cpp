#include "Enemies/Managers/SGEnemyDropManager.h"

#include "Enemies/Characters/SGEnemyCharacter.h"
#include "Utils/SGUtilObjectPoolManager.h"
#include "Kismet/GameplayStatics.h"
#include "Pickups/SGPickUp.h"
#include "Utils/SGObjectPoolSubsystem.h"

ASGEnemyDropManager::ASGEnemyDropManager()
{
	PrimaryActorTick.bCanEverTick = false;
}


void ASGEnemyDropManager::BeginPlay()
{
	Super::BeginPlay();

	if (!EnemyDropDataTable)
	{
		return;
	}

	ObjectPoolSubsystem = GetGameInstance()->GetSubsystem<USGObjectPoolSubsystem>();

	auto PreloadEnemyDropData = [&](const FName&, const FEnemyDropInfo& Drop)
	{
		if (!Drop.EnemyClass)
		{
			return;
		}
		TSubclassOf<AActor> DropClass = TSubclassOf(Drop.PickUpClass);
		ObjectPoolSubsystem->InitializePool(DropClass, Drop.PickUpCount);
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
			if (AActor* PooledObject = ObjectPoolSubsystem->GetPooledObject(DropClass))
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
