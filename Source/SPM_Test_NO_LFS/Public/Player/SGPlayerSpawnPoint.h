
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGPlayerSpawnPoint.generated.h"

UCLASS()
class SPM_TEST_NO_LFS_API ASGPlayerSpawnPoint : public AActor
{
	GENERATED_BODY()

public:
	ASGPlayerSpawnPoint();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* Root;
	
	UPROPERTY(EditAnywhere, Category = "SpawnNumber")
	int32 SpawnNumber = 0;

	UPROPERTY(EditAnywhere, Category = "SpawnNumber")
	class UBillboardComponent* Billboard;
};
