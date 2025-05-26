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

	Sub->OnBindAttribute.AddDynamic(this, &USGUpgradeWidget::ConstructEntries);
	Sub->OnUpgrade.AddDynamic(this, &USGUpgradeWidget::ConstructEntries);

	GetGameInstance()->GetSubsystem<USGUpgradeGuardSubsystem>()->OnCountAddToCount.AddDynamic(this, &USGUpgradeWidget::ConstructEntries);
}

//TODO: Borde hantera logik för att uppgradera attributen i UI:t också (ta bort det ansvaret från USGUpgradeEntryTile)
void USGUpgradeWidget::ConstructEntries()
{
	UE_LOG(LogTemp, Display, TEXT("USGUpgradeWidget::ConstructEntries"));
	if (!EntryTileClass || !EntriesBox || !CategoryWidgetClass || !MiscEntriesBox)
	{		
		return;
	}
	
	//TODO: Pool/Optimera för sätt att återanvända widgets
	EntriesBox->ClearChildren(); 
	MiscEntriesBox->ClearChildren(); 
	CategoryWidgets.Empty();

	const USGUpgradeSubsystem* Sub = GetGameInstance()->GetSubsystem<USGUpgradeSubsystem>();
	if (!ensureMsgf(Sub, TEXT("Upgrade Subsystem was nullptr")))
	{
		return;
	}	

	for (const FSGUpgradeEntry& UpgradeEntry : Sub->GetUpgradeEntries())
	{
		USGUpgradeEntryTile* Tile = CreateWidget<USGUpgradeEntryTile>(this, EntryTileClass);
		if (!ensureMsgf(Tile, TEXT("Tile was nullptr")))
		{
			continue;
		}
		Tile->SetupEntry(UpgradeEntry);

		// Check if we already have a category widget
		USGUpgradeCategoryWidget* CategoryWidget = nullptr;
		if (CategoryWidgets.Contains(UpgradeEntry.Category))
		{
			CategoryWidget = CategoryWidgets[UpgradeEntry.Category];
		}
		else
		{
			// Create a new category widget
			CategoryWidget = CreateWidget<USGUpgradeCategoryWidget>(this, CategoryWidgetClass);
			if (!ensureMsgf(CategoryWidget, TEXT("Failed to create category widget")))
			{
				continue;
			}

			// Add to map and vertical box
			CategoryWidgets.Add(UpgradeEntry.Category, CategoryWidget);
			if (UpgradeEntry.Category == "Player") // TODO: justera hårdkodad lösning - fält i datatabellen för kategori/tab?
			{
				EntriesBox->AddChild(CategoryWidget);
			}
			else
			{
				MiscEntriesBox->AddChild(CategoryWidget);
			}
		}

		// Now safe to call SetupEntry on a valid widget
		CategoryWidget->SetupEntry(UpgradeEntry.Category, Tile);
	}
}

void USGUpgradeWidget::VisibilityChanged(const ESlateVisibility NewVisibility)
{
	UE_LOG(LogTemp, Display, TEXT("VisibilityChanged"));
	ConstructEntries();	
}