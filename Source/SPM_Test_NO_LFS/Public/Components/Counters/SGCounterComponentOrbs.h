#pragma once

#include "CoreMinimal.h"
#include "SGCounterComponent.h"
#include "SGCounterComponentOrbs.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SPM_TEST_NO_LFS_API USGCounterComponentOrbs : public USGCounterComponent
{
	GENERATED_BODY()

public:
	USGCounterComponentOrbs();

	UFUNCTION(BlueprintPure, Category = "Counter Component")
	float GetOrbCount() const;

	UFUNCTION(BlueprintCallable, Category = "Counter Component")
	void AddOrbs(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Counter Component")
	void RemoveOrbs(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Counter Component")
	bool CanRemoveOrbs(float Amount) const;

protected:
	virtual void BeginPlay() override;
	
	virtual void ProcessPickup(AActor* Pickup) override;

	UPROPERTY(EditAnywhere, Category = "Counter Properties")
	float OrbCount = 0.0f;
};