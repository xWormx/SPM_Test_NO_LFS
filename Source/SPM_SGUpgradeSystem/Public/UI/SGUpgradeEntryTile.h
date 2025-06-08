#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/SGAttribute.h"
#include "SGUpgradeEntryTile.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnteredEntryTile, USGUpgradeEntryTile*, EntryTile);
class UButton;
class UImage;
class UTextBlock;

UCLASS()
class SPM_SGUPGRADESYSTEM_API USGUpgradeEntryTile : public UUserWidget
{
	GENERATED_BODY()

public:

	USGUpgradeEntryTile(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	
	UFUNCTION(BlueprintCallable, Category="Upgrades")
	void SetupEntry(const FSGUpgradeEntry& Entry);

	FOnEnteredEntryTile OnEnteredEntryTile;
private:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	bool CanUpgrade() const;

	UFUNCTION()
	void HandleClicked();

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent) override;
public:
	UFUNCTION()
	void HandleButtonState();

protected:
	FSGUpgradeEntry BoundEntry;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UImage> Icon;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UTextBlock> UpgradeText;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UTextBlock> UpgradeLevelText;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UTextBlock> UpgradeCurrentValueText;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UTextBlock> UpgradeMultiplierText;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UTextBlock> UpgradeCostText;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UButton> UpgradeButton;

	friend class USGUpgradeWidget;
};
