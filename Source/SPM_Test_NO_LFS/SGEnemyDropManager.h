#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Containers/Map.h"
#include "SGEnemyDropManager.generated.h"

class ASGPickUp;

//För att smidigare ange vilken typ av fiende som ska droppa vad, mängd och potentiell spawnradius om flera av samma typ ska spawna
USTRUCT(BlueprintType, Category = "Item Drop")
struct FSGEnemyDrop
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - PickUp Type", meta = (ToolTip = "Använd motsvarande Blueprint-klass för att spawna"))
	TSubclassOf<ASGPickUp> PickUpClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - PickUp Count")
	int PickUpCount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - PickUp Count")
	float SpawnRadius = 10.0f;
};

//Workaround för att kunna ha TArray i en TMap 
USTRUCT(BlueprintType, Category = "Item Drop")
struct FSGEnemyDropContainer
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Enemy Drops")
	TArray<FSGEnemyDrop> EnemyDrops;
};

class ASGEnemyCharacter;

UCLASS()
class SPM_TEST_NO_LFS_API ASGEnemyDropManager : public AActor
{
	GENERATED_BODY()

public:
	ASGEnemyDropManager();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	//Anropas när fienden dör
	UFUNCTION(BlueprintCallable, Category = "UFunction - Drop Item")
	void DropItem(ASGEnemyCharacter* EnemyCharacter);

private:
	
	//TODO: Om tid och värt - använd data table istället
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - PickUp Type", meta = (AllowPrivateAccess = "true", ToolTip = "Använd motsvarande Blueprint-klass som nyckel"))
	TMap<TSubclassOf<ASGEnemyCharacter>, FSGEnemyDropContainer> EnemyDropMap;
};