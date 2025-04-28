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

	FOnAttributeModified OnAttributeModified; 
};

USTRUCT(BlueprintType)
struct FSGUpgradeData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ToolTip = "Antalet Orbs"))
	int32 Cost = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ToolTip = "Används som: value += multiplier"))
	float Multiplier = 100.0f;	
};

USTRUCT(BlueprintType)
struct FSGAttributeData : public FTableRowBase
{
	GENERATED_BODY()	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon = nullptr;

	//TODO: pris per upgrade? Ska fler "typer" av upgrades utanför float stödjas?
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> Upgrades;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSGUpgradeData> UpgradeData;
};


