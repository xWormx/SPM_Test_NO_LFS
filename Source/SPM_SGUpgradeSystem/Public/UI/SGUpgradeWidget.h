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
	UFUNCTION()
	void UpdateCategoryWidget(USGUpgradeEntryTile* UpgradeEntryTile);
protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UVerticalBox> EntriesBox;

	UPROPERTY(EditAnywhere, Category="Upgrades")
	TSubclassOf<USGUpgradeEntryTile> EntryTileClass;

	UPROPERTY(EditAnywhere, Category="Upgrades")
	TSubclassOf<USGUpgradeCategoryWidget> CategoryWidgetClass;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<USGUpgradeCategoryWidget> UpgradeCategoryWidget;

	UFUNCTION()
	void ConstructEntries();

	UFUNCTION()
	void VisibilityChanged(ESlateVisibility NewVisibility);

private:
	TMap<FName, TObjectPtr<USGUpgradeEntryTile>> UpgradeEntryTileMap;
};
