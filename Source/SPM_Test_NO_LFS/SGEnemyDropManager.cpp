#include "SGEnemyDropManager.h"

#include "SGEnemyCharacter.h"
#include "SGPickUp.h"

ASGEnemyDropManager::ASGEnemyDropManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASGEnemyDropManager::BeginPlay()
{
	Super::BeginPlay();	
}

void ASGEnemyDropManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASGEnemyDropManager::DropItem(ASGEnemyCharacter* EnemyCharacter)
{
	if (!EnemyCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyCharacter is null."));
		return;
	}

	const FSGEnemyDropContainer* Drops = EnemyDropMap.Find(EnemyCharacter->GetClass());

	if (!Drops)
	{
		UE_LOG(LogTemp, Warning, TEXT("No drops found for the given enemy."));
		return;
	}

	for (const auto& Drop : Drops->EnemyDrops)
	{
		if (!Drop.PickUpClass)
		{
			continue;
		}
		
		FVector SpawnLocation = EnemyCharacter->GetActorLocation();
		FRotator SpawnRotation = FRotator::ZeroRotator;
		
		for (int i = 0; i < Drop.PickUpCount; ++i)
		{
			//Den bortkommenterade koden är inte ordentligt testad än
			/* FVector RandomOffset = FMath::VRand() * FMath::RandRange(0.0f, Drop.SpawnRadius);
			FVector RandomSpawnLocation = SpawnLocation + RandomOffset;
			GetWorld()->SpawnActor<ASGPickUp>(Drop.PickUpClass, RandomSpawnLocation, SpawnRotation);*/
			GetWorld()->SpawnActor<ASGPickUp>(Drop.PickUpClass, SpawnLocation, SpawnRotation);
		}
	}
}

