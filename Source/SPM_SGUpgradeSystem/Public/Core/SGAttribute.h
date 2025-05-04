#pragma once
#include "CoreMinimal.h"
#include "SGAttribute.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttributeModified);

USTRUCT(BlueprintType)
struct FSGAttribute
{
	GENERATED_BODY()	
		
	TWeakObjectPtr<UObject> Owner;
	FProperty* Property = nullptr;

	FName RowName; //Måste vara samma som i "row name" som sätts i själva datatabellen.
	int32 CurrentUpgradeLevel = 0;
	float InitialValue = 0.f; 
	FOnAttributeModified OnAttributeModified; 
};

USTRUCT(BlueprintType)
struct FSGUpgradeData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ToolTip = "Antalet Orbs som krävs för att uppgradera. Priset multipliceras"))
	int32 Cost = 10.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ToolTip = "-1 = Ingen maxgräns"))
	int32 MaxNumberOfUpgrades = -1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ToolTip = "Används som: value += InitialValue * Multiplier"))
	float Multiplier = 0.1f; // Procenttal ökning av värdet	
};

USTRUCT(BlueprintType)
struct FSGAttributeData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSGUpgradeData Data;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName DisplayName;
	
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    TSoftObjectPtr<UTexture2D> Icon = TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Engine/VREditor/Devices/Vive/UE4_Logo.UE4_Logo")));
};

USTRUCT(BlueprintType)
struct FSGUpgradeEntry : public FSGUpgradeData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName DisplayName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon = nullptr;
};

