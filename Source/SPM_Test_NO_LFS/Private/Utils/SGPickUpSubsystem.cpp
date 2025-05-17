#include "Utils/SGPickUpSubsystem.h"

#include "Enemies/Characters/SGEnemyCharacter.h"
#include "Enemies/Managers/SGEnemyDropManager.h"
#include "Pickups/SGPickUp.h"
#include "Utils/SGDeveloperSettings.h"
#include "Utils/SGObjectPoolSubsystem.h"

void USGPickUpSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const USGDeveloperSettings* DeveloperSettings = GetDefault<USGDeveloperSettings>();
	if (!DeveloperSettings)
	{
		return;
	}
	const UDataTable* EnemyDrops = Cast<UDataTable>(DeveloperSettings->EnemyDropsDataTable.TryLoad());

	auto PreloadEnemyDropData = [&](const FName&, const FEnemyDropInfo& Drop)
	{
		if (!Drop.EnemyClass)
		{
			return;
		}
		EnemyDropMap.FindOrAdd(Drop.EnemyClass).Add(Drop);
	};

	EnemyDrops->ForeachRow<FEnemyDropInfo>(TEXT("Initialize Enemy Drop Data"), PreloadEnemyDropData);
}

void USGPickUpSubsystem::DropItem(ASGEnemyCharacter* EnemyCharacter) const
{
	if (!EnemyCharacter)
	{
		return;
	}

	TArray<FEnemyDropInfo> const* DropInfoArray = EnemyDropMap.Find(EnemyCharacter->GetClass());
	if (!DropInfoArray)
	{
		return;
	}

	USGObjectPoolSubsystem* ObjectPoolSubsystem = GetGameInstance()->GetSubsystem<USGObjectPoolSubsystem>();

	const FVector SpawnLocation = EnemyCharacter->GetActorLocation();
	const FRotator SpawnRotation = FRotator::ZeroRotator;

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
				constexpr float CollisionBoxSize = 10.0f;
				FVector DropLocation = SpawnLocation;

				UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(PooledObject->GetRootComponent());
				FCollisionShape CollShape = PrimComp
					                            ? FCollisionShape::MakeBox(PrimComp->Bounds.BoxExtent)
					                            : FCollisionShape::MakeBox(FVector(CollisionBoxSize));

				FCollisionQueryParams Params;
				Params.AddIgnoredActor(EnemyCharacter);
				bool bHasCollision = GetWorld()->OverlapBlockingTestByChannel(DropLocation, FQuat::Identity, ECC_WorldStatic, CollShape, Params);

				if (bHasCollision && Drop.SpawnCondition.SpawnRadius > 0.0f)
				{
					bool bFoundValidLocation = false;

					for (int32 Attempt = 0; Attempt < Drop.SpawnCondition.MaxSpawnAttempts; ++Attempt)
					{
						constexpr float HeightOffset = 0.5f;
						FVector RandomDir = FMath::VRand();
						RandomDir.Z = FMath::Abs(RandomDir.Z) * HeightOffset;

						float RandomDistance = FMath::FRandRange(0.f, Drop.SpawnCondition.SpawnRadius);
						FVector TestLocation = SpawnLocation + (RandomDir * RandomDistance);

						bHasCollision = GetWorld()->OverlapBlockingTestByChannel(TestLocation, FQuat::Identity, ECC_WorldStatic, CollShape, Params);

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
}

