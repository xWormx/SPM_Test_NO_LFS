#pragma once

#include "CoreMinimal.h"
#include "SGCounterComponent.h"
#include "SGCounterComponentHealth.generated.h"

class USGHealthComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SPM_TEST_NO_LFS_API USGCounterComponentHealth : public USGCounterComponent
{
	GENERATED_BODY()

public:
	USGCounterComponentHealth();

	UFUNCTION(BlueprintCallable, Category = "Counter Component")
	void IncreaseHealth(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Counter Component")
	void UseHealthBuffer(float Amount);

	//TODO: Eventuellt ta bort - är bara en redirector till HealthComponent
	UFUNCTION(BlueprintPure, Category = "Counter Component")
	float GetCurrentHealth() const;

	UFUNCTION(BlueprintPure, Category = "Counter Component")
	float GetBufferedHealth() const;

protected:

	virtual void BeginPlay() override;
	
	virtual void ProcessPickup(AActor* Pickup) override;
	
	//TODO: Byt ut så counter syns i en widget
	virtual void LogCounter() override;	

	UPROPERTY(EditAnywhere, Category = "Counter Properties")
	USGHealthComponent* HealthComponent;
	
	// TODO: Implementera Timer
	UPROPERTY(EditAnywhere, Category = "Counter Properties")
	float HealthBuffer = 0.0f;	
};