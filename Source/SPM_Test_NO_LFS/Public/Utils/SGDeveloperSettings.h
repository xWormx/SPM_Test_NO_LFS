﻿#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "SGDeveloperSettings.generated.h"

UCLASS(config=Game, DefaultConfig)
class SPM_TEST_NO_LFS_API USGDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:

	virtual FName GetCategoryName() const override;

#if WITH_EDITOR
	virtual FText GetSectionText() const override;
	virtual FText GetSectionDescription() const override;
#endif

	UPROPERTY(Config, EditAnywhere, Category="Enemy Drops Data Table", meta=(AllowedClasses="/Script/Engine.DataTable"))
	FSoftObjectPath EnemyDropsDataTable;

	UPROPERTY(Config, EditAnywhere, Category="UI| Weapon UI Data Table", meta=(AllowedClasses="/Script/Engine.DataTable"))
	FSoftObjectPath WeaponUIDataTable;

	UPROPERTY(Config, EditAnywhere, Category="UI| Styling", DisplayName="Selected Theme")
	FDataTableRowHandle StylingThemeRow;

	UPROPERTY(Config, EditAnywhere, Category="UI| Styling", meta=(AllowedClasses="/Script/Engine.DataTable"))
	FSoftObjectPath StylingThemeDataTable;

	UPROPERTY(Config, EditAnywhere, Category="UI| String Table", meta=(AllowedClasses="/Script/Engine.StringTable"))
	FSoftObjectPath UIStringTable;
};
