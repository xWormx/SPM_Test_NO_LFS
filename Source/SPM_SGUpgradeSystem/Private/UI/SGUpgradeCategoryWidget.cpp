// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SGUpgradeCategoryWidget.h"

#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "UI/SGUpgradeEntryTile.h"

void USGUpgradeCategoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void USGUpgradeCategoryWidget::SetupEntry(const FName& Category, USGUpgradeEntryTile* UpgradeEntry)
{
	UpgradeCategoryText->SetText(FText::FromName(Category));
	EntriesBox->AddChildToVerticalBox(UpgradeEntry);
}
