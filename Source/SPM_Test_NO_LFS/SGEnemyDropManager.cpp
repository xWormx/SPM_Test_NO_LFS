#include "SGEnemyDropManager.h"

#include "SGEnemyCharacter.h"
#include "SGPickUp.h"

ASGEnemyDropManager::ASGEnemyDropManager()
{
	PrimaryActorTick.bCanEverTick = false;
}


void ASGEnemyDropManager::DropItem(ASGEnemyCharacter* EnemyCharacter) const
{
	if (!EnemyCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyCharacter is null."));
		return;
	}
	if (!EnemyDropDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyDropDataTable is null."));
		return;
	}
	
	const FVector SpawnLocation = EnemyCharacter->GetActorLocation();
	const FRotator SpawnRotation = FRotator::ZeroRotator;

	const FString DebugMessage = TEXT("Iterate Enemy Drop Data");
	auto DropLoot = [&]([[maybe_unused]] const FName& Key, const FEnemyDropInfo& Drop)
	{
		if (EnemyCharacter->GetClass()->IsChildOf(Drop.EnemyClass))
		{
			GetWorld()->SpawnActor<ASGPickUp>(Drop.PickUpClass, SpawnLocation, SpawnRotation);
		}
	};
		
	EnemyDropDataTable->ForeachRow<FEnemyDropInfo>(DebugMessage, DropLoot);
}

