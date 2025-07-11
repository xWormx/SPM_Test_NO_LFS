﻿#pragma once

#include "CoreMinimal.h"
#include "SGCounterComponent.h"
#include "SGCounterComponentHealth.generated.h"

UDELEGATE(Blueprintable)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShieldChanged);

class USGHealthComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SPM_TEST_NO_LFS_API USGCounterComponentHealth : public USGCounterComponent
{
	GENERATED_BODY()

public:
	USGCounterComponentHealth();

	UFUNCTION(BlueprintCallable, Category = "UProperty - Counter Component")
	void IncreaseHealth(float Amount);

	UFUNCTION(BlueprintCallable, Category = "UProperty - Counter Component")
	void StartHealthBuffer(float NewHealth);

	UFUNCTION(BlueprintCallable, Category = "UProperty - Counter Component")
	void UseHealthBuffer(float Amount);

	//TODO: Eventuellt ta bort - är bara en redirector till HealthComponent
	UFUNCTION(BlueprintPure, Category = "UProperty - Counter Component")
	float GetCurrentHealth() const;

	UFUNCTION(BlueprintPure, Category = "UProperty - Counter Component")
	float GetBufferedHealth() const;

	UFUNCTION(BlueprintPure, Category = "UProperty - Counter Component")
	float GetBufferCapacity() const;

protected:
	virtual void BeginPlay() override;
	virtual void ProcessPickup(AActor* Pickup) override;

	UPROPERTY(VisibleAnywhere, Category = "UProperty - Counter Properties")
	USGHealthComponent* HealthComponent;
	
	UPROPERTY(EditAnywhere, Category = "UProperty - Counter Properties| Shield", meta = (AllowPrivateAccess = "true"))
	float HealthBuffer = 0.0f;

	UPROPERTY(EditAnywhere, Category = "UProperty - Counter Properties| Shield", meta = (AllowPrivateAccess = "true"))
	float HealthBufferCapacity = 100.0f; 

private:
	FTimerHandle HealthBufferTimerHandle;

	UPROPERTY(EditAnywhere, Category = "UProperty - Counter Properties|Countdown Timer")
	float HealthBufferTimerInterval = 0.0f;

	UPROPERTY(VisibleAnywhere, Category = "UProperty - Counter Properties|Countdown Timer")
	float HealthBufferDecreaseRate = 1.0f; // Mängd att minska med per sekund om buffert är aktiv
	
	UFUNCTION()
	void DecreaseHealthBuffer(float Amount);
	void StartHealthBufferTimer();

public:
	UPROPERTY(BlueprintAssignable, Category = "Health Component")
	FOnShieldChanged OnShieldChanged;

 };
