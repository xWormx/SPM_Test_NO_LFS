#include "UI/Widgets/SGWeaponsHUD.h"

#include "SPM_Test_NO_LFS.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Gear/Weapons/SGGun.h"
#include "UI/Widgets/SGWeaponEntry.h"
#include "UI/Widgets/WidgetData/WeaponData.h"
#include "Utils/SGDeveloperSettings.h"

void USGWeaponsHUD::NativeConstruct()
{
	Super::NativeConstruct();

	WeaponsDataTable = Cast<UDataTable>(GetDefault<USGDeveloperSettings>()->WeaponUIDataTable.TryLoad());

	if (!WeaponsDataTable)
	{
		EMMA_LOG(Error, TEXT("❤️WeaponDataTable is null"));
		return;
	}

	TArray<FWeaponData*> WeaponData;
	WeaponsDataTable->GetAllRows<FWeaponData>(TEXT("WeaponData"), WeaponData);

	ConstructWeaponEntries(WeaponData);
}

void USGWeaponsHUD::ConstructWeaponEntries(const TArray<FWeaponData*>& WeaponData)
{
	if (!ensure(EntryTileClass))
	{
		return;
	}

	for (const FWeaponData* Data : WeaponData)
	{
		if (!Data)
		{
			EMMA_LOG(Error, TEXT("❤️WeaponData is null"));
			continue;
		}

		EMMA_LOG(Log, TEXT("❤️WeaponData"));
		USGWeaponEntry* WeaponEntry = CreateWidget<USGWeaponEntry>(this, EntryTileClass.Get());
		WeaponEntry->WeaponIcon->SetBrushFromTexture(Data->WeaponIcon.LoadSynchronous());
		WeaponEntriesBox->AddChild(WeaponEntry);
		WeaponEntries.Add(WeaponEntry);
	}
}

void USGWeaponsHUD::SetAvailableWeapons(const TArray<ASGGun*>& Weapons)
{
	if (WeaponEntries.Num() == 0)
	{
		return;
	}

	// Assign weapons to entries
	for (int i = 0; i < WeaponEntries.Num(); ++i)
	{
		if (!Weapons.IsValidIndex(i))
		{
			continue;
		}
		USGWeaponEntry* WeaponEntry = WeaponEntries[i];
		ASGGun* Gun = Weapons[i];
		if (!WeaponEntry || !Gun)
		{
			continue;
		}
		FText WeaponName = Gun->GetWeaponDisplayName();
		const int32 AmmoClip = Gun->GetAmmoClip();
		const int32 AmmoStock = Gun->GetAmmoStock();

		WeaponEntry->WeaponNameTextBlock->SetText(WeaponName);
		WeaponEntry->AmmoClipTextBlock->SetText(GetAmmoClipText(AmmoClip));
		WeaponEntry->EmptyAmmoBorder->SetBrushColor(GetAmmoColor(AmmoClip));
		if (AmmoStock >= 0)
		{
			WeaponEntry->AmmoStockTextBlock->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			static const FText InfiniteAmmoStock = FText::FromString(TEXT("/∞"));
			WeaponEntry->AmmoStockTextBlock->SetText(InfiniteAmmoStock);
		}
		FName WeaponNameKey = FName(*WeaponName.ToString());
		WeaponEntriesMap.Add(WeaponNameKey, WeaponEntry);
	}

	// Remove entries without assigned weapons (iterate in reverse)
	for (int i = WeaponEntries.Num() - 1; i >= 0; --i)
	{
		if (!Weapons.IsValidIndex(i) || !Weapons[i])
		{
			if (WeaponEntries[i])
			{
				WeaponEntries[i]->RemoveFromParent();
			}
			WeaponEntries.RemoveAt(i);
		}
	}
}
void USGWeaponsHUD::ChangeWeapon(const int32 WeaponIndex, ASGGun* Gun)
{
	if (!WeaponEntries.IsValidIndex(WeaponIndex))
	{
		return;
	}
	for (int32 i = 0; i < WeaponEntries.Num(); ++i)
	{
		USGWeaponEntry* WeaponEntry = WeaponEntries[i];
		if (!WeaponEntry)
		{
			continue;
		}
		WeaponEntry->SetIsEnabled(i == WeaponIndex);
	}
}

void USGWeaponsHUD::UpdateWeapon(int32 WeaponIndex, ASGGun* Gun)
{
	if (!Gun)
	{
		return;
	}

	TObjectPtr<USGWeaponEntry> WeaponEntry = WeaponEntries[WeaponIndex];
	if (!WeaponEntry)
	{
		return;
	}

	WeaponEntry->AmmoClipTextBlock->SetText(GetAmmoClipText(Gun->GetAmmoClip()));
//	WeaponEntry->AmmoClipTextBlock->SetColorAndOpacity(GetAmmoColor(Gun->GetAmmoClip()));
	WeaponEntry->EmptyAmmoBorder->SetBrushColor(GetAmmoColor(Gun->GetAmmoClip()));

}

void USGWeaponsHUD::UpdateAmmo(int32 AmmoAmount, ASGGun* Gun)
{
	FName WeaponNameKey = FName(*Gun->GetWeaponDisplayName().ToString());
	TObjectPtr<USGWeaponEntry>* WeaponEntry = WeaponEntriesMap.Find(WeaponNameKey);
	if (!WeaponEntry)
	{
 		return;
	}

	WeaponEntry->Get()->SetIsEnabled(true);
	const int32 ClipAmmo = Gun->GetAmmoClip();
	WeaponEntry->Get()->AmmoStockTextBlock->SetText(GetAmmoClipText(ClipAmmo));
//	WeaponEntry->Get()->AmmoStockTextBlock->SetColorAndOpacity(GetAmmoColor(ClipAmmo));
	WeaponEntry->Get()->EmptyAmmoBorder->SetBrushColor(GetAmmoColor(ClipAmmo));
	WeaponEntry->Get()->SetIsEnabled(false);

}

FColor USGWeaponsHUD::GetAmmoColor(const int32 ClipAmmo)
{
	//static constexpr FColor GrayColor = FColor(128, 128, 128);
	static constexpr FColor RedColor = FColor(255, 0, 0, 45);
	static constexpr FColor Transparent = FColor(0, 0, 0, 45);
	return ClipAmmo != 0 ? Transparent : RedColor;
}

FText USGWeaponsHUD::GetAmmoClipText(const int32 ClipAmmo)
{
	static const FText InfiniteAmmoText = FText::FromString(TEXT("∞"));
	return ClipAmmo < 0 ? InfiniteAmmoText : FText::AsNumber(ClipAmmo);
}
