#include "UI/SGUpgradeEntryTile.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Core/SGAttribute.h"
#include "Core/SGUpgradeSubsystem.h"

void USGUpgradeEntryTile::SetupEntry(const FSGUpgradeEntry& Entry)
{
	BoundRowName = Entry.RowName;

	if (Icon && Entry.Icon)
	{
		Icon->SetBrushFromTexture(Entry.Icon);
	}

	if (UpgradeText)
	{
		const FText EntryCost = FText::AsNumber(Entry.Cost);
		const FText EntryMultiplier = FText::AsNumber(Entry.Multiplier);	
		UpgradeText->SetText(FText::Format(FText::FromString("Cost: {0} Multiplier: {1}"), EntryCost, EntryMultiplier));
	}	
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
	USGUpgradeSubsystem* UpgradeSubsystem = GetGameInstance()->GetSubsystem<USGUpgradeSubsystem>();
	if (!UpgradeSubsystem)
	{
		return;
	}
	UpgradeSubsystem->RequestUpgrade(true, BoundRowName);
}
