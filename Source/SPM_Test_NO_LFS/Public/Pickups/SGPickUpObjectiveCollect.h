#pragma once

#include "CoreMinimal.h"
#include "Objectives/SGObjectiveInterface.h"
#include "SGPickUp.h"
#include "SGPickUpObjectiveCollect.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCollected, UObject*, ObjectiveInterfaceImplementor);

UCLASS()
class SPM_TEST_NO_LFS_API ASGPickUpObjectiveCollect : public ASGPickUp, public ISGObjectiveInterface
{
	GENERATED_BODY()

public:
	ASGPickUpObjectiveCollect();
	FOnCollected OnCollected;

	virtual void OnPickup_Implementation() override;
	virtual EObjectiveType GetObjectiveType() override { return EObjectiveType::EOT_Collect; }
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleOverlap(AActor* OverlappedActor, AActor* OtherActor);
	
	UPROPERTY(EditAnywhere, Category = "UPROPERTY - PickUp")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category = "UPROPERTY - PickUp")
	USoundBase* PickUpSound;
};
