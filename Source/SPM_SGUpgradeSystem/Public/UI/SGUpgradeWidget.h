#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SGUpgradeWidget.generated.h"

class USGUpgradeCategoryWidget;
class USGUpgradeEntryTile;
class UVerticalBox;

UCLASS()
class SPM_SGUPGRADESYSTEM_API USGUpgradeWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	USGUpgradeWidget(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer){}	
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UVerticalBox> EntriesBox;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UVerticalBox> MiscEntriesBox;

	UPROPERTY(EditAnywhere, Category="Upgrades")
	TSubclassOf<USGUpgradeEntryTile> EntryTileClass;

	UPROPERTY(EditAnywhere, Category="Upgrades")
	TSubclassOf<USGUpgradeCategoryWidget> CategoryWidgetClass;

	UFUNCTION()
	void ConstructEntries();

private:
	TMap<FName, TObjectPtr<USGUpgradeCategoryWidget>> CategoryWidgets;
};
