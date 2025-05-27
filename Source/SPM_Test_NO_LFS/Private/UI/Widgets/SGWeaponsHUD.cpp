#include "UI/Widgets/SGWeaponsHUD.h"

#include "SPM_Test_NO_LFS.h"
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
			continue;
		}

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
		UpdateEntryAmmo(AmmoClip, WeaponEntry);

		if (AmmoStock < 0)
		{
			static const FText InfiniteAmmoStock = FText::FromString(TEXT("/∞"));
			WeaponEntry->AmmoStockTextBlock->SetText(InfiniteAmmoStock);
		}
		else
		{
			WeaponEntry->AmmoStockTextBlock->SetVisibility(ESlateVisibility::Collapsed);
		}

		FName WeaponNameKey = FName(*WeaponName.ToString());
		WeaponEntriesMap.Add(WeaponNameKey, WeaponEntry);
	}

	// Remove entries without assigned weapons (iterate in reverse)
	for (int i = WeaponEntries.Num() - 1; i >= 0; --i)
	{
		if (!Weapons.IsValidIndex(i) || !Weapons[i])
		{
			//Alternatively use Visibility::Collapsed if spot should be used for e.g. Lenade Grauncher
			if (WeaponEntries[i])
			{
				WeaponEntries[i]->RemoveFromParent();
			}
			WeaponEntries.RemoveAt(i);
		}
	}
}

void USGWeaponsHUD::ChangeWeapon(const int32 WeaponIndex, [[maybe_unused]] ASGGun* Gun)
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
	UpdateEntryAmmo(Gun->GetAmmoClip(), WeaponEntry.Get());
}

void USGWeaponsHUD::ReloadWeapon(int32 WeaponIndex, ASGGun* Gun)
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
	static const FText ReloadingText = FText::FromString(TEXT("Reloading..."));
	WeaponEntry.Get()->AmmoClipTextBlock->SetText(ReloadingText);

	int32 AmmoClip = Gun->GetAmmoClip();
	FTimerDelegate ReloadDelegate;
	FTimerHandle ReloadTimerHandle;
	ReloadDelegate.BindLambda([WeaponEntry, Gun, this, AmmoClip, &ReloadTimerHandle]
	{
		if (AmmoClip < Gun->GetAmmoClip())
		{
			WeaponEntry.Get()->WeaponNameTextBlock->SetText(Gun->GetWeaponDisplayName());
			UpdateEntryAmmo(Gun->GetAmmoClip(), WeaponEntry.Get());
			GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
		}
	});

	constexpr float ReloadTime = 0.1f; // Gun->GetReloadTime();
	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, ReloadDelegate, ReloadTime, true);
}
void USGWeaponsHUD::UpdateAmmo([[maybe_unused]] int32 AmmoAmount, ASGGun* Gun)
{
	if (!Gun)
	{
		return;
	}

	FName WeaponNameKey = FName(*Gun->GetWeaponDisplayName().ToString());
	TObjectPtr<USGWeaponEntry>* WeaponEntry = WeaponEntriesMap.Find(WeaponNameKey);
	if (!WeaponEntry)
	{
 		return;
	}
	const int32 ClipAmmo = Gun->GetAmmoClip();

	FTimerDelegate AmmoUpdateDelegate;
	FTimerHandle AmmoUpdateTimerHandle;
	AmmoUpdateDelegate.BindLambda([WeaponEntry, this]
	{
		WeaponEntry->Get()->AmmoClipTextBlock->SetColorAndOpacity(GetAmmoStateChangeColor(0));
	});
	GetWorld()->GetTimerManager().SetTimer(AmmoUpdateTimerHandle, AmmoUpdateDelegate, 1.f, false);

	if (WeaponEntry->Get()->GetIsEnabled())
	{
		WeaponEntry->Get()->AmmoClipTextBlock->SetColorAndOpacity(GetAmmoStateChangeColor(ClipAmmo));
		UpdateEntryAmmo(ClipAmmo, WeaponEntry->Get());
		return;
	}

	WeaponEntry->Get()->SetIsEnabled(true);
	WeaponEntry->Get()->AmmoClipTextBlock->SetColorAndOpacity(GetAmmoStateChangeColor(ClipAmmo));
	UpdateEntryAmmo(ClipAmmo, WeaponEntry->Get());
	WeaponEntry->Get()->SetIsEnabled(false);
}

void USGWeaponsHUD::UpdateEntryAmmo(const int32 AmmoClip, USGWeaponEntry* WeaponEntry)
{
	if (!WeaponEntry)
	{
		return;
	}
	WeaponEntry->AmmoClipTextBlock->SetText(GetAmmoClipText(AmmoClip));
	WeaponEntry->EmptyAmmoBorder->SetBrushColor(GetAmmoStatusBorderColor(AmmoClip));
}

FColor USGWeaponsHUD::GetAmmoStatusBorderColor(const int32 ClipAmmo)
{
	static constexpr FColor OutOfAmmoColor = FColor(255, 0, 0, 45);
	static constexpr FColor DefaultColor = FColor(0, 0, 0, 45);
	return ClipAmmo != 0 ? DefaultColor : OutOfAmmoColor;
}

FColor USGWeaponsHUD::GetAmmoStateChangeColor(const int32 ClipAmmo)
{
	static constexpr FColor IncreasedAmmo = FColor(25, 155, 25, 255);
	static constexpr FColor DefaultColor = FColor(222, 222, 222, 255);
	return ClipAmmo != 0 ? IncreasedAmmo : DefaultColor;
}

FText USGWeaponsHUD::GetAmmoClipText(const int32 ClipAmmo)
{
	static const FText InfiniteAmmoText = FText::FromString(TEXT("∞"));
	return ClipAmmo < 0 ? InfiniteAmmoText : FText::AsNumber(ClipAmmo);
}
