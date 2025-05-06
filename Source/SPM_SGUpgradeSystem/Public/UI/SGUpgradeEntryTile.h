#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/SGAttribute.h"
#include "SGUpgradeEntryTile.generated.h"

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
	
private:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void HandleClicked();
public:
	UFUNCTION()
	void HandleButtonState();

protected:
	FSGUpgradeEntry BoundEntry;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UImage> Icon; // TODO: ikon för ägare eller attribut?
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UTextBlock> UpgradeText;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UTextBlock> UpgradeMultiplierText;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UTextBlock> UpgradeCostText;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UButton> UpgradeButton;
};
