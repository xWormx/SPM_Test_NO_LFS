#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SGUpgradeEntryTile.generated.h"

class UButton;
struct FSGUpgradeEntry;
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

	UFUNCTION()
	void HandleClicked();

protected:
	FName BoundRowName;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UImage> Icon; // TODO: ikon för ägare eller attribut?
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UTextBlock> UpgradeText;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UButton> UpgradeButton;
};
