#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SGUpgradeGuardSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCountAddToCount);

UDELEGATE(Blueprintable)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCountModified);


//TODO: 😷 Temporär lösning - Kolla om man ska kunna sätta "constraints" hos respektive attribut för uppgradering eller om sån logik ska ske utanför. 
UCLASS()
class SPM_SGUPGRADESYSTEM_API USGUpgradeGuardSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	
	USGUpgradeGuardSubsystem(){};

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable, Category = "UFunction - Upgrade System")
	void AddToCount(float Amount);

	UFUNCTION(BlueprintCallable, Category = "UFunction - Upgrade System")
	void RemoveFromCount(float Amount);
	
	UFUNCTION(BlueprintCallable, Category = "UFunction - Upgrade System")
	bool CanUpgradeBasedOnCount(float UpgradeCost) const;

	UFUNCTION(BlueprintPure, Category = "UFunction - Upgrade System")
	float GetCount() const;
	void SetCount(float NewCount);
	void ResetCount();

private:

	UPROPERTY()
	float Count;

	UPROPERTY()
	float DefaultValue = 10.0f;

public:
	FOnCountAddToCount OnCountAddToCount;
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnCountModified OnCountModified;
};
