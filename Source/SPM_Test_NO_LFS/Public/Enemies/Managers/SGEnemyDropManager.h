#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Containers/Map.h"
#include "Engine/DataTable.h"
#include "SGEnemyDropManager.generated.h"

/*
class ASGPickUp;
class ASGEnemyCharacter;

USTRUCT(BlueprintType)
struct FEnemyDropSpawnCondition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Enemy Drop")
	float SpawnRadius = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Enemy Drop", meta=(ToolTip = "Antalet gånger som den försöker hitta en alternativ position utan kollision inom angiven radius innan den ger upp och kör på platsen där fienden dog."))
	int32 MaxSpawnAttempts = 10;
};

USTRUCT(BlueprintType)
struct FEnemyDrop
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Enemy Drop", meta = (ToolTip = "Använd motsvarande Blueprint-klass för att spawna"))
	TSubclassOf<ASGPickUp> PickUpClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Enemy Drop")
	int32 PickUpCount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Enemy Drop", meta = (ToolTip = "1 = 100% drop chance"))
	float DropChance = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Enemy Drop")
	float SpawnRadius = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Enemy Drop", meta=(ToolTip = "Antalet gånger som den försöker hitta en alternativ position utan kollision inom angiven radius innan den ger upp och kör på platsen där fienden dog."))
	int32 MaxSpawnAttempts = 10;
};

//TODO: Se över layout för hantering av enemy drops så att t.ex. en health-pickup alltid har samma spawning conditions
USTRUCT(BlueprintType, Category = "Enemy Drop Info")
struct FEnemyDropInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Enemy Drop")
	TSubclassOf<ASGEnemyCharacter> EnemyClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Enemy Drop", meta = (ToolTip = "Använd motsvarande Blueprint-klass för att spawna"))
	TSubclassOf<ASGPickUp> PickUpClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Enemy Drop")
	int32 PickUpCount = 1;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Enemy Drop")
	float SpawnRadius = 10.0f;#1#

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Enemy Drop", meta = (ToolTip = "1 = 100% drop chance"))
	float DropChance = 1.f;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Enemy Drop")
	TArray<FEnemyDrop> EnemyDrops;#1#

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Enemy Drop", meta = (ToolTip = "Används ifall pickupen kolliderar med något statiskt när den spawnas"))
	FEnemyDropSpawnCondition SpawnCondition;
};
*/

UCLASS()
class SPM_TEST_NO_LFS_API ASGEnemyDropManager : public AActor
{
	GENERATED_BODY()

/*
public:
	ASGEnemyDropManager();

	//Anropas när fienden dör
	UFUNCTION(BlueprintCallable, Category = "UFunction - Enemy Drops")
	void DropItem(ASGEnemyCharacter* EnemyCharacter) const;

protected:
	virtual void BeginPlay() override;
	
private:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UProperty - Enemy Drops", meta = (AllowPrivateAccess = "true"))
	UDataTable* EnemyDropDataTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UProperty - Enemy Drops", meta = (AllowPrivateAccess = "true"))
	class USGObjectPoolSubsystem* ObjectPoolSubsystem;

	TMap<TSubclassOf<ASGEnemyCharacter>, TArray<FEnemyDropInfo>> EnemyDropMap;*/
};