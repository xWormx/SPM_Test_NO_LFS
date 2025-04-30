#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SGUpgradeWidget.generated.h"

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
	/** Container in your UMG (e.g. VerticalBox or GridPanel) */
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UVerticalBox> EntriesBox;

	/** The widget class for each entry tile */
	UPROPERTY(EditAnywhere, Category="Upgrades")
	TSubclassOf<USGUpgradeEntryTile> EntryTileClass;

	UFUNCTION()
	void ConstructEntries();
};
