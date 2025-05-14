#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SGWeaponsHUD.generated.h"

UCLASS()
class SPM_TEST_NO_LFS_API USGWeaponsHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Weapons")
	void UpdWeaponName(const FText& Name);

	UFUNCTION(BlueprintImplementableEvent, Category = "Weapons")
	void UpdAmmoClip(int32 ClipAmmo);

	UFUNCTION(BlueprintImplementableEvent, Category = "Weapons")
	void UpdAmmoStock(int32 StockAmmo);
};
