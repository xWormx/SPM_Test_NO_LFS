#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "SGUpgradeDeveloperSettings.generated.h"

UCLASS(config=Game, DefaultConfig)
class SPM_SGUPGRADESYSTEM_API USGUpgradeDeveloperSettings : public UDeveloperSettings
{	
	GENERATED_BODY()
public:

	virtual FName GetCategoryName() const override;
	
#if WITH_EDITOR
	virtual FText GetSectionText() const override;
	virtual FText GetSectionDescription() const override;
#endif

	UPROPERTY(Config, EditAnywhere, Category="Upgrades Data Table", meta=(AllowedClasses="/Script/Engine.DataTable"))
	FSoftObjectPath UpgradeDataTable;		
};
