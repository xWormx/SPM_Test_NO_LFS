#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SGWeaponsHUD.generated.h"

class UVerticalBox;
class ASGGun;
struct FWeaponData;
class USGWeaponEntry;
class UHorizontalBox;
class UImage;
class UTextBlock;

UCLASS()
class SPM_TEST_NO_LFS_API USGWeaponsHUD : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;
	void ConstructWeaponEntries(const TArray<FWeaponData*>& WeaponData);
	void SetAvailableWeapons(const TArray<ASGGun*>& Weapons);

	UFUNCTION()
	void ChangeWeapon(int32 WeaponIndex, ASGGun* Gun);

	UFUNCTION()
	void UpdateWeapon(int32 WeaponIndex, ASGGun* Gun);

	UFUNCTION()
	void ReloadWeapon(int32 WeaponIndex, ASGGun* Gun);

	UFUNCTION()
	void UpdateAmmo(int32 AmmoAmount, ASGGun* Gun);
	void UpdateEntryAmmo(int32 AmmoClip, USGWeaponEntry* WeaponEntry);

	UFUNCTION(BlueprintImplementableEvent, Category = "Weapons")
	void UpdWeaponName(const FText& Name);

	UFUNCTION(BlueprintImplementableEvent, Category = "Weapons")
	void UpdAmmoClip(int32 ClipAmmo);

	UFUNCTION(BlueprintImplementableEvent, Category = "Weapons")
	void UpdAmmoStock(int32 StockAmmo);

protected:

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UVerticalBox> WeaponEntriesBox;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USGWeaponEntry> EntryTileClass;

	TArray<TObjectPtr<USGWeaponEntry>> WeaponEntries;
	TMap<FName, TObjectPtr<USGWeaponEntry>> WeaponEntriesMap;

	/*UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	TObjectPtr<UTextBlock> WeaponNameTextBlock;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	TObjectPtr<UTextBlock> AmmoClipTextBlock;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	TObjectPtr<UTextBlock> AmmoStockTextBlock;*/

private:
	static FColor GetAmmoStatusBorderColor(int32 ClipAmmo);
	static FColor GetAmmoStateChangeColor(int32 ClipAmmo);
	static FText GetAmmoClipText(int32 ClipAmmo);

	UPROPERTY()
	UDataTable* WeaponsDataTable = nullptr;
};
