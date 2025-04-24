#pragma once

#include "CoreMinimal.h"
#include "SGPickUp.h"
#include "SGPickUpObjectiveCollect.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCollected, ASGPickUpObjectiveCollect*, Collectable);

UCLASS()
class SPM_TEST_NO_LFS_API ASGPickUpObjectiveCollect : public ASGPickUp
{
	GENERATED_BODY()

public:
	ASGPickUpObjectiveCollect();
	FOnCollected OnCollected;

	virtual void OnPickup_Implementation() override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleOverlap(AActor* OverlappedActor, AActor* OtherActor);
	
	UPROPERTY(EditAnywhere, Category = "UPROPERTY - PickUp")
	class UStaticMeshComponent* Mesh;
};
