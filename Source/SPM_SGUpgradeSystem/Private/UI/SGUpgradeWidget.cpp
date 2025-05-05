#include "UI/SGUpgradeWidget.h"
#include "Components/VerticalBox.h"
#include "Core/SGUpgradeSubsystem.h"
#include "UI/SGUpgradeEntryTile.h"

void USGUpgradeWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ConstructEntries();
	//TODO: Sub till events	
}

//TODO: Borde hantera logik för att uppgradera attributen i UI:t också (ta bort det ansvaret från USGUpgradeEntryTile)
void USGUpgradeWidget::ConstructEntries()
{
	if (!EntriesBox)
	{
		return;
	}
	if (!EntryTileClass)
	{
		return;
	}
	
	EntriesBox->ClearChildren(); //TODO: Pool/Optimera för sätt att återanvända widgets

	const USGUpgradeSubsystem* Sub = GetGameInstance()->GetSubsystem<USGUpgradeSubsystem>();
	if (!Sub)
	{
		return;
	}

	for (const FSGUpgradeEntry& UpgradeEntry : Sub->GetUpgradeEntries())
	{
		USGUpgradeEntryTile* Tile = CreateWidget<USGUpgradeEntryTile>(this, EntryTileClass);
		if (!Tile)
		{
			continue;
		}
		Tile->SetupEntry(UpgradeEntry);
		EntriesBox->AddChildToVerticalBox(Tile); //Kan också använda AddChild
	}	
}
