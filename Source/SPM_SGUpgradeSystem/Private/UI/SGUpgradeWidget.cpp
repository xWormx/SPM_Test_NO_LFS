#include "UI/SGUpgradeWidget.h"

#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Core/SGUpgradeGuardSubsystem.h"
#include "Core/SGUpgradeSubsystem.h"
#include "UI/SGUpgradeCategoryWidget.h"
#include "UI/SGUpgradeEntryTile.h"

void USGUpgradeWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ConstructEntries();

	OnVisibilityChanged.AddDynamic(this, &USGUpgradeWidget::VisibilityChanged);

	USGUpgradeSubsystem* Sub = GetGameInstance()->GetSubsystem<USGUpgradeSubsystem>();
	if (!ensureMsgf(Sub, TEXT("Upgrade Subsystem was nullptr")))
	{
		return;
	}
	//TODO: check if all these are even needed?
	Sub->OnBindAttribute.AddDynamic(this, &USGUpgradeWidget::ConstructEntries);
	Sub->OnUpgrade.AddDynamic(this, &USGUpgradeWidget::ConstructEntries);
	GetGameInstance()->GetSubsystem<USGUpgradeGuardSubsystem>()->OnCountModified.AddDynamic(this, &USGUpgradeWidget::ConstructEntries);
}

void USGUpgradeWidget::UpdateCategoryWidget(USGUpgradeEntryTile* UpgradeEntryTile)
{
	if (!UpgradeEntryTile || !UpgradeCategoryWidget)
	{
		return;
	}
	const FSGUpgradeEntry UpgradeEntryData = UpgradeEntryTile->BoundEntry;
    UpgradeCategoryWidget->SetupEntry(&UpgradeEntryData);
}

void USGUpgradeWidget::ConstructEntries()
{
	if (!EntryTileClass || !EntriesBox || !CategoryWidgetClass)
	{		
		return;
	}

	const USGUpgradeSubsystem* Sub = GetGameInstance()->GetSubsystem<USGUpgradeSubsystem>();
	if (!ensureMsgf(Sub, TEXT("Upgrade Subsystem was nullptr")))
	{
		return;
	}	

	for (const FSGUpgradeEntry& UpgradeEntry : Sub->GetUpgradeEntries())
	{
		USGUpgradeEntryTile* Tile = nullptr;
		if (TObjectPtr<USGUpgradeEntryTile>* TilePtr = UpgradeEntryTileMap.Find(UpgradeEntry.DisplayName)) //Try to reuse existing Tile
		{
			Tile = *TilePtr;
		}
		if (!Tile) // If this is the first time creating a Tile
		{
			Tile = CreateWidget<USGUpgradeEntryTile>(this, EntryTileClass);
			UpgradeEntryTileMap.Add(UpgradeEntry.DisplayName, Tile);
			Tile->SetIsFocusable(true);
			Tile->OnEnteredEntryTile.AddDynamic(this, &USGUpgradeWidget::UpdateCategoryWidget);
			EntriesBox->AddChild(Tile);
		}
		if (!ensureMsgf(Tile, TEXT("Tile was nullptr")))
		{
			continue;
		}
		Tile->SetupEntry(UpgradeEntry);
	}
}

void USGUpgradeWidget::VisibilityChanged([[maybe_unused]] const ESlateVisibility NewVisibility)
{
	ConstructEntries();
}
