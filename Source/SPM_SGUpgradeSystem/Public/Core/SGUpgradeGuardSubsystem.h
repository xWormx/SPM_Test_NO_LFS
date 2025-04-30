#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SGUpgradeGuardSubsystem.generated.h"

//TODO: 😷 Temporär lösning - Kolla om man ska kunna sätta "constraints" hos respektive attribut för uppgradering eller om sån logik ska ske utanför. 
UCLASS()
class SPM_SGUPGRADESYSTEM_API USGUpgradeGuardSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	USGUpgradeGuardSubsystem(){};
	
	UFUNCTION(BlueprintCallable, Category = "UFunction - Upgrade System")
	void AddToCount(float Amount);

	UFUNCTION(BlueprintCallable, Category = "UFunction - Upgrade System")
	void RemoveFromCount(float Amount);
	
	UFUNCTION(BlueprintCallable, Category = "UFunction - Upgrade System")
	bool CanUpgradeBasedOnCount(float UpgradeCost) const;

	UFUNCTION(BlueprintPure, Category = "UFunction - Upgrade System")
	float GetCount() const;
private:
	UPROPERTY()
	float Count = 1.0f;
};
