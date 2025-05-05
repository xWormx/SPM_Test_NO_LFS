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
	/*if (!Icon || !Entry.Icon|| !UpgradeText)
	{
		return;
	}*/
	
	Icon->SetBrushFromTexture(Entry.Icon);
	
	const FText EntryCost = FText::AsNumber(Entry.Cost);
	const FText EntryMultiplier = FText::AsNumber(Entry.Multiplier);
	const FText EntryName = FText::FromName(Entry.DisplayName);
	const FText EntryCategory = FText::FromName(Entry.Category);
	
	
	UpgradeText->SetText( FText::Format(FText::FromString("{0}: {1} (+{2}%) {3}"), EntryCategory, EntryName, EntryMultiplier, EntryCost));
}

void USGUpgradeEntryTile::NativeConstruct()
{
	Super::NativeConstruct();
	if (UpgradeButton)
	{
		UpgradeButton->OnClicked.AddDynamic(this, &USGUpgradeEntryTile::HandleClicked);
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
	
	UpgradeSubsystem->RequestUpgrade(UpgradeGuardInstance->CanUpgradeBasedOnCount(BoundEntry.Cost), BoundEntry.DisplayName);
}
