#pragma once

#include "CoreMinimal.h"
#include "SGCounterComponent.h"
#include "SGCounterComponentHealth.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNoHealth, float, NewHealth);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SPM_TEST_NO_LFS_API USGCounterComponentHealth : public USGCounterComponent
{
	GENERATED_BODY()

public:
	USGCounterComponentHealth();
	
	UPROPERTY(BlueprintAssignable, Category = "Counter Component")
	FOnNoHealth OnNoHealth;	

	UFUNCTION(BlueprintPure, Category = "Counter Component")
	float GetCurrentHealth() const;

	UFUNCTION(BlueprintPure, Category = "Counter Component")
	float GetBufferedHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Counter Component")
	void IncreaseHealth(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Counter Component")
	void DecreaseHealth(float Amount);
	
protected:
	virtual void ProcessPickup(AActor* Pickup) override;
	
	//TODO: Byt ut så counter syns i en widget
	virtual void LogCounter() override;

	// TODO: Byt ut till HealthComponent
	UPROPERTY(EditAnywhere, Category = "Counter Properties")
	float CurrentHealth = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Counter Properties")
	float MaxHealth = 100.0f;

	// TODO: Implementera Timer
	UPROPERTY(EditAnywhere, Category = "Counter Properties")
	float HealthBuffer = 0.0f;
};