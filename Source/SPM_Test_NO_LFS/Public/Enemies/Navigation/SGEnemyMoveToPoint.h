
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGEnemyMoveToPoint.generated.h"

UCLASS()
class SPM_TEST_NO_LFS_API ASGEnemyMoveToPoint : public AActor
{
	GENERATED_BODY()

public:
	ASGEnemyMoveToPoint();

protected:
	virtual void BeginPlay() override;


	//Used for drawing debug lines etc in editor
#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
#endif

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBillboardComponent* Billboard;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveToPoint")
	FName BeaconTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveToPoint")
	float Priority = 1.0f;
	
	bool bIsCurrent = false;
	bool bIsPrevious = false;*/
};
