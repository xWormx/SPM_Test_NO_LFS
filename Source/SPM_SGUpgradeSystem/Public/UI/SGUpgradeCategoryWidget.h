// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGUpgradeEntryTile.h"
#include "Blueprint/UserWidget.h"
#include "SGUpgradeCategoryWidget.generated.h"

class UVerticalBox;
class UTextBlock;
/**
 * 
 */
UCLASS()
class SPM_SGUPGRADESYSTEM_API USGUpgradeCategoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	USGUpgradeCategoryWidget(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer){}
	virtual void NativeConstruct() override;
	void SetupEntry(const FName& Category, USGUpgradeEntryTile* UpgradeEntry);

protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UTextBlock> UpgradeCategoryText;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UVerticalBox> EntriesBox;

};
