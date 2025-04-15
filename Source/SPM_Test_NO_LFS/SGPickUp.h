#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGPickUp.generated.h"

UCLASS(Abstract)
class SPM_TEST_NO_LFS_API ASGPickUp : public AActor
{
	GENERATED_BODY()

public:
	ASGPickUp();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintNativeEvent, Category = "PickUp")
	void OnPickup(); 
	
	virtual void OnPickup_Implementation();
	
	UFUNCTION(BlueprintPure, Category = "PickUp")
	float GetPickupValue() const { return PickUpValue; }

	UFUNCTION(BlueprintCallable, Category ="PickUp")
	void SetPickUpValue(float Value);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PickUp Properties")
	class USphereComponent* CollisionSphere;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PickUp Properties")
	float PickUpValue = 10.0f; //Random defaultvärde bara
};