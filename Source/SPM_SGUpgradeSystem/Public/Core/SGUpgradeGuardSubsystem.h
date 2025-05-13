#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SGUpgradeGuardSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCountAddToCount);


//TODO: 😷 Temporär lösning - Kolla om man ska kunna sätta "constraints" hos respektive attribut för uppgradering eller om sån logik ska ske utanför. 
UCLASS()
class SPM_SGUPGRADESYSTEM_API USGUpgradeGuardSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	
	USGUpgradeGuardSubsystem(){};

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category = "UFunction - Upgrade System")
	void AddToCount(float Amount);

	UFUNCTION(BlueprintCallable, Category = "UFunction - Upgrade System")
	void RemoveFromCount(float Amount);
	
	UFUNCTION(BlueprintCallable, Category = "UFunction - Upgrade System")
	bool CanUpgradeBasedOnCount(float UpgradeCost) const;

	UFUNCTION(BlueprintPure, Category = "UFunction - Upgrade System")
	float GetCount() const;

protected:
	void ResetCount(UWorld* World);

private:

	UPROPERTY()
	float Count = 200.0f;

	UPROPERTY()
	float DefaultValue = 200.0f;

	FDelegateHandle PostLevelLoadHandle;

public:
	FOnCountAddToCount OnCountAddToCount;
};
