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

	UFUNCTION(BlueprintNativeEvent, Category = "UFUNCTION - PickUp")
	void OnPickup(); 
	
	virtual void OnPickup_Implementation();
	
	UFUNCTION(BlueprintPure, Category = "UFUNCTION - PickUp")
	float GetPickupValue() const;

	UFUNCTION(BlueprintCallable, Category ="UFUNCTION - PickUp")
	void SetPickUpValue(float Value);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UPROPERTY - PickUp")
	class USphereComponent* CollisionSphere;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UPROPERTY - PickUp")
	float PickUpValue = 10.0f; //Random defaultvärde bara
};