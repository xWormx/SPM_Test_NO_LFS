#include "UI/SGUpgradeWidget.h"
#include "Components/VerticalBox.h"
#include "Core/SGUpgradeGuardSubsystem.h"
#include "Core/SGUpgradeSubsystem.h"
#include "UI/SGUpgradeEntryTile.h"

void USGUpgradeWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ConstructEntries();
	//TODO: Sub till events

	USGUpgradeSubsystem* Sub = GetGameInstance()->GetSubsystem<USGUpgradeSubsystem>();
	if (!ensureMsgf(Sub, TEXT("Upgrade Subsystem was nullptr")))
	{
		return;
	}
	Sub->OnBindAttribute.AddDynamic(this, &USGUpgradeWidget::ConstructEntries);
}

//TODO: Borde hantera logik för att uppgradera attributen i UI:t också (ta bort det ansvaret från USGUpgradeEntryTile)
void USGUpgradeWidget::ConstructEntries()
{	
	if (!EntryTileClass || !EntriesBox)
	{		
		return;
	}	
	
	EntriesBox->ClearChildren(); //TODO: Pool/Optimera för sätt att återanvända widgets

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
		
		EntriesBox->AddChildToVerticalBox(Tile); //Kan också använda AddChild
	}	
}
