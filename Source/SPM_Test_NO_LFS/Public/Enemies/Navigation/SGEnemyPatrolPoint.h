

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGEnemyPatrolPoint.generated.h"

UCLASS()
class SPM_TEST_NO_LFS_API ASGEnemyPatrolPoint : public AActor
{
	GENERATED_BODY()

public:
	
	ASGEnemyPatrolPoint();

protected:
	
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBillboardComponent* Billboard;
};
