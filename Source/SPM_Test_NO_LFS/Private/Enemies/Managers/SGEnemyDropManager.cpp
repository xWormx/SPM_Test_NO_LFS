#include "Enemies/Managers/SGEnemyDropManager.h"

#include "Enemies/Characters/SGEnemyCharacter.h"
#include "Pickups/SGPickUp.h"
#include "Utils/SGObjectPoolSubsystem.h"

/*ASGEnemyDropManager::ASGEnemyDropManager()
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

		EnemyDropMap.FindOrAdd(Drop.EnemyClass).Add(Drop);
	};
	EnemyDropDataTable->ForeachRow<FEnemyDropInfo>(TEXT("Initialize Enemy Drop Data"), PreloadEnemyDropData);
}

void ASGEnemyDropManager::DropItem(ASGEnemyCharacter* EnemyCharacter) const
{
	if (!EnemyCharacter || !EnemyDropDataTable)
	{
		return;
	}

	constexpr float HeightOffset = 0.5f;
	constexpr float CollisionBoxSize = 10.0f;

	const FVector SpawnLocation = EnemyCharacter->GetActorLocation();
	const FRotator SpawnRotation = FRotator::ZeroRotator;

	TArray<FEnemyDropInfo> const* DropInfoArray = EnemyDropMap.Find(EnemyCharacter->GetClass());
	if (!DropInfoArray)
	{
		return;
	}

	for (const FEnemyDropInfo Drop : *DropInfoArray)
	{
		if (FMath::FRand() > Drop.DropChance)
		{
			continue;
		}
		for (int32 i = 0; i < Drop.PickUpCount; ++i)
		{
			TSubclassOf<AActor> DropClass = TSubclassOf(Drop.PickUpClass);
			if (AActor* PooledObject = ObjectPoolSubsystem->GetPooledObject(DropClass))
			{
				FVector DropLocation = SpawnLocation;

				UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(PooledObject->GetRootComponent());
				FCollisionShape CollShape = PrimComp
					                            ? FCollisionShape::MakeBox(PrimComp->Bounds.BoxExtent)
					                            : FCollisionShape::MakeBox(FVector(CollisionBoxSize));

				FCollisionQueryParams Params;
				Params.AddIgnoredActor(EnemyCharacter);
				bool bHasCollision = GetWorld()->OverlapBlockingTestByChannel(
					DropLocation, FQuat::Identity, ECC_WorldStatic, CollShape, Params);

				if (bHasCollision && Drop.SpawnCondition.SpawnRadius > 0.0f)
				{
					bool bFoundValidLocation = false;

					for (int32 Attempt = 0; Attempt < Drop.SpawnCondition.MaxSpawnAttempts; ++Attempt)
					{
						FVector RandomDir = FMath::VRand();
						RandomDir.Z = FMath::Abs(RandomDir.Z) * HeightOffset;

						float RandomDistance = FMath::FRandRange(0.f, Drop.SpawnCondition.SpawnRadius);
						FVector TestLocation = SpawnLocation + (RandomDir * RandomDistance);

						bHasCollision = GetWorld()->OverlapBlockingTestByChannel(
							TestLocation, FQuat::Identity, ECC_WorldStatic, CollShape, Params);

						if (!bHasCollision)
						{
							DropLocation = TestLocation;
							bFoundValidLocation = true;
							break;
						}
					}

					if (!bFoundValidLocation)
					{
						DropLocation = SpawnLocation;
					}
				}

				PooledObject->SetActorLocation(DropLocation);
				PooledObject->SetActorRotation(SpawnRotation);
				PooledObject->SetActorHiddenInGame(false);
				PooledObject->SetActorEnableCollision(true);
			}
		}
	}

	/*
	const FString DebugMessage = TEXT("Iterate Enemy Drop Data");
	auto DropLoot = [&]([[maybe_unused]] const FName& Key, const FEnemyDropInfo& Drop)
	{
		if (!EnemyCharacter->GetClass()->IsChildOf(Drop.EnemyClass))
		{
			return;
		}

		if (FMath::FRand() > Drop.DropChance)
		{
			return;
		}

		for (int32 i = 0; i < Drop.PickUpCount; ++i)
		{
			TSubclassOf<AActor> DropClass = TSubclassOf(Drop.PickUpClass);
			if (AActor* PooledObject = ObjectPoolSubsystem->GetPooledObject(DropClass))
			{
				FVector DropLocation = SpawnLocation;

				UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(PooledObject->GetRootComponent());
				FCollisionShape CollShape = PrimComp
					                            ? FCollisionShape::MakeBox(PrimComp->Bounds.BoxExtent)
					                            : FCollisionShape::MakeBox(FVector(CollisionBoxSize));

				FCollisionQueryParams Params;
				Params.AddIgnoredActor(EnemyCharacter);
				bool bHasCollision = GetWorld()->OverlapBlockingTestByChannel(
					DropLocation, FQuat::Identity, ECC_WorldStatic, CollShape, Params);

				if (bHasCollision && Drop.SpawnCondition.SpawnRadius > 0.0f)
				{
					bool bFoundValidLocation = false;

					for (int32 Attempt = 0; Attempt < Drop.SpawnCondition.MaxSpawnAttempts; ++Attempt)
					{
						FVector RandomDir = FMath::VRand();
						RandomDir.Z = FMath::Abs(RandomDir.Z) * HeightOffset;

						float RandomDistance = FMath::FRandRange(0.f, Drop.SpawnRadius);
						FVector TestLocation = SpawnLocation + (RandomDir * RandomDistance);

						bHasCollision = GetWorld()->OverlapBlockingTestByChannel(
							TestLocation, FQuat::Identity, ECC_WorldStatic, CollShape, Params);

						if (!bHasCollision)
						{
							DropLocation = TestLocation;
							bFoundValidLocation = true;
							break;
						}
					}

					if (!bFoundValidLocation)
					{
						DropLocation = SpawnLocation;
					}
				}

				PooledObject->SetActorLocation(DropLocation);
				PooledObject->SetActorRotation(SpawnRotation);
				PooledObject->SetActorHiddenInGame(false);
				PooledObject->SetActorEnableCollision(true);
			}
		}
	};

	EnemyDropDataTable->ForeachRow<FEnemyDropInfo>(DebugMessage, DropLoot);#1#
}*/
