#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SGHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNoHealth, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHurt, float, NewHealth);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SPM_TEST_NO_LFS_API USGHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USGHealthComponent();

	UPROPERTY(BlueprintAssignable, Category = "Health Component")
	FOnNoHealth OnNoHealth;
	
	UPROPERTY(BlueprintAssignable, Category = "Health Component")
	FOnHurt OnHurt;

	UFUNCTION(BlueprintCallable, Category = "Health Component")
	void Heal(float HealAmount);
	
	UFUNCTION(BlueprintPure, Category = "Health Component")
	float GetCurrentHealth() const;

	UFUNCTION(BlueprintPure, Category = "Health Component")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Health Component")
	void SetCurrentHealth(float NewHealth);	
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Health Component", meta = (AllowPrivateAccess = "true"))
	float MaxHealth = 100.f;

	UPROPERTY(EditAnywhere, Category = "Health Component")
	float CurrentHealth = 100.f;

	UFUNCTION()
	void DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* Instigator, AActor* DamageCauser);

};
