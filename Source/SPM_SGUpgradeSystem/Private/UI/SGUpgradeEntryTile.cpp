#include "UI/SGUpgradeEntryTile.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Core/SGAttribute.h"
#include "Core/SGUpgradeGuardSubsystem.h"
#include "Core/SGUpgradeSubsystem.h"

void USGUpgradeEntryTile::SetupEntry(const FSGUpgradeEntry& Entry)
{
	BoundEntry = Entry;
	
	Icon->SetBrushFromTexture(Entry.Icon);
	
	const FText EntryCost = FText::AsNumber(Entry.Cost);
	const FText EntryMultiplier = FText::AsNumber(Entry.Multiplier);
	const FText EntryName = FText::FromName(Entry.DisplayName);

	UpgradeText->SetText( FText::Format(FText::FromString("{0}:"), EntryName));
	UpgradeMultiplierText->SetText(FText::Format(FText::FromString("+{0}%"), EntryMultiplier));
	UpgradeCostText->SetText(FText::Format(FText::FromString("{0}"), EntryCost));
	UpgradeButton->OnClicked.AddDynamic(this, &USGUpgradeEntryTile::HandleClicked);
	HandleButtonState();
}

void USGUpgradeEntryTile::NativeConstruct()
{
	Super::NativeConstruct();
}

void USGUpgradeEntryTile::NativeDestruct()
{
	Super::NativeDestruct();
	if (UpgradeButton)
	{
		UpgradeButton->OnClicked.RemoveDynamic(this, &USGUpgradeEntryTile::HandleClicked);
	}
}

void USGUpgradeEntryTile::HandleClicked()
{
	const USGUpgradeSubsystem* UpgradeSubsystem = GetGameInstance()->GetSubsystem<USGUpgradeSubsystem>();
	if (!UpgradeSubsystem)
	{
		return;
	}

	const USGUpgradeGuardSubsystem* UpgradeGuardInstance = GetGameInstance()->GetSubsystem<USGUpgradeGuardSubsystem>();
	if (!UpgradeGuardInstance)
	{
		return;
	}
	UpgradeSubsystem->RequestUpgrade(UpgradeGuardInstance->CanUpgradeBasedOnCount(BoundEntry.Cost), BoundEntry.RowName, BoundEntry.Category);
}

void USGUpgradeEntryTile::HandleButtonState()
{
	const USGUpgradeGuardSubsystem* UpgradeGuardInstance = GetGameInstance()->GetSubsystem<USGUpgradeGuardSubsystem>();
	if (!UpgradeGuardInstance)
	{
		return;
	}

	const bool bCanUpgrade = UpgradeGuardInstance->CanUpgradeBasedOnCount(BoundEntry.Cost);
	const FText TooltipText = bCanUpgrade ? FText::FromString("Upgrade") : FText::FromString("Not enough Orbs");

	UpgradeButton->SetToolTipText(TooltipText);
	UpgradeButton->SetIsEnabled(bCanUpgrade);
}
