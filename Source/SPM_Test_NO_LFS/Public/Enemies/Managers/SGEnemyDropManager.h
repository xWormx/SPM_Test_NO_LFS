#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Containers/Map.h"
#include "Engine/DataTable.h"
#include "SGEnemyDropManager.generated.h"

class ASGPickUp;
class ASGEnemyCharacter;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UProperty - Enemy Drop")
	float SpawnRadius = 10.0f;
};

UCLASS()
class SPM_TEST_NO_LFS_API ASGEnemyDropManager : public AActor
{
	GENERATED_BODY()

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
};