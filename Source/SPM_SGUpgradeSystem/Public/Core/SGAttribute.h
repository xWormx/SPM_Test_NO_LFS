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
	int32 CurrentUpgradeLevel = 1;
	float InitialValue = 0.f; 
	FOnAttributeModified OnAttributeModified;
	FName Category;
	bool bSave = true;
};

USTRUCT(BlueprintType)
struct FSGDependentAttribute
{
	GENERATED_BODY()

	TWeakObjectPtr<UObject> Owner;
	FProperty* Property = nullptr;
	bool bOverrideOnModified = false; // Ifall nuvarande värde ska spegla det modifierade värdet. Om inte så kommer den ändras procentuellt.
};

// For
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DescriptionText = FText::FromString("Some description here...");
};

//UI representation
USTRUCT(BlueprintType)
struct FSGUpgradeEntry : public FSGUpgradeData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon = nullptr;

	int32 CurrentUpgradeLevel = 0;	
	float CurrentValue = 0.f;
	
	FName Category;
	FName RowName;
	FText DescriptionText;
};

//Save attribut
USTRUCT(Blueprintable)
struct FSGUpgradePersistentData
{
	GENERATED_BODY()
	FName PropertyName;
	FName RowName;
	FName Category;
	int32 CurrentUpgradeLevel;
	float InitialValue;
};

//TMap workaround
USTRUCT(Blueprintable)
struct FSGSavedAttributeEntry
{
	GENERATED_BODY()

	FString ClassNameKey = FString();
	TArray<FSGUpgradePersistentData> PersistentUpgrades = TArray<FSGUpgradePersistentData>();
};

USTRUCT(Blueprintable)
struct FSGSavedAttributes
{
	GENERATED_BODY()

	float Orbs = 10.0f;
	TArray<FSGSavedAttributeEntry> SomeAttributes = TArray<FSGSavedAttributeEntry>();
};
