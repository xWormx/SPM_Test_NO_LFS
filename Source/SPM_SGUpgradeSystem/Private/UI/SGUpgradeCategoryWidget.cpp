

#include "UI/SGUpgradeCategoryWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "UI/SGUpgradeEntryTile.h"

void USGUpgradeCategoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void USGUpgradeCategoryWidget::SetupEntry(const FSGUpgradeEntry* Entry)
{
	if (!Entry)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetupEntry called with nullptr Entry!"));
		return;
	}
	Icon->SetBrushFromTexture(Entry->Icon);
	UpgradeCategoryText->SetText(FText::FromName(Entry->DisplayName));
	DescriptionText->SetText(Entry->DescriptionText);
}
