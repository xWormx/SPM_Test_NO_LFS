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

	// ENTRY TO TEXT
	const FText EntryCost = FText::AsNumber(Entry.Cost);
	const FText EntryMultiplier = FText::AsNumber(FMath::Abs(Entry.Multiplier));
	const FText EntryName = FText::FromName(Entry.DisplayName);
	const FText EntryCurrentValue = FText::AsNumber(Entry.CurrentValue);
	const FText EntryCurrentLevel = FText::AsNumber(Entry.CurrentUpgradeLevel);
	const FText EntryMaxLevel = FText::AsNumber(Entry.MaxNumberOfUpgrades);
	FText MaxNumberOfUpgrades = Entry.MaxNumberOfUpgrades == -1
		                            ? FText::Format(FText::FromString("{0}/Infinite"), EntryCurrentLevel)
		                            : FText::Format(FText::FromString("{0}/{1}"), EntryCurrentLevel, EntryMaxLevel);

	// SET TEXT
	UpgradeText->SetText( FText::Format(FText::FromString("{0}:"), EntryName));
	UpgradeCurrentValueText->SetText(FText::Format(FText::FromString("{0}"), EntryCurrentValue));
	UpgradeMultiplierText->SetText(FText::Format(FText::FromString("+{0}"), EntryMultiplier));
	UpgradeCostText->SetText(FText::Format(FText::FromString("{0}"), EntryCost));
	UpgradeLevelText->SetText(MaxNumberOfUpgrades);

	// ICON
	Icon->SetBrushFromTexture(Entry.Icon);

	if (Entry.CurrentUpgradeLevel == Entry.MaxNumberOfUpgrades)
	{
		UpgradeButton->SetVisibility(ESlateVisibility::Hidden);
		UpgradeMultiplierText->SetVisibility(ESlateVisibility::Hidden);
		this->SetToolTipText(FText::FromString("Maxed out"));
		return;
	}	
	
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

bool USGUpgradeEntryTile::CanUpgrade() const
{
	const USGUpgradeGuardSubsystem* UpgradeGuard = GetGameInstance()->GetSubsystem<USGUpgradeGuardSubsystem>();
	return UpgradeGuard ? UpgradeGuard->CanUpgradeBasedOnCount(BoundEntry.Cost) : false;
}

void USGUpgradeEntryTile::HandleClicked()
{
	const USGUpgradeSubsystem* Upgrader = GetGameInstance()->GetSubsystem<USGUpgradeSubsystem>();
	if (!Upgrader)
	{
		return;
	}

	Upgrader->RequestUpgrade(CanUpgrade(), BoundEntry.RowName, BoundEntry.Category);
}

void USGUpgradeEntryTile::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	UE_LOG(LogTemp, Display, TEXT("USGUpgradeEntryTile::NativeOnMouseEnter - %s"), *BoundEntry.DisplayName.ToString());
	OnEnteredEntryTile.Broadcast(this);
}

void USGUpgradeEntryTile::NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnAddedToFocusPath(InFocusEvent);
	OnEnteredEntryTile.Broadcast(this);
}

void USGUpgradeEntryTile::HandleButtonState()
{
	const bool bCanUpgrade = CanUpgrade();
	const FText TooltipText = bCanUpgrade ? FText::FromString("Upgrade") : FText::FromString("Not enough Orbs");

	UpgradeButton->SetToolTipText(TooltipText);
	UpgradeButton->SetIsEnabled(bCanUpgrade);
}
