#pragma once

#include "CoreMinimal.h"
#include "Core/SGAttribute.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SGUpgradeSubsystem.generated.h"

struct FSGAUpgradeResult
{
	int32 Level;
	float Cost;
	bool bUpgraded;
};

//TODO: Snygga till. Flera för att undersöka vilka som lär vilja användas för UI osv
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpgradeCost, float, UpgradeCost);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpgradeLevel, float, UpgradeLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpgrade);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBindAttribute);

UCLASS()
class SPM_SGUPGRADESYSTEM_API USGUpgradeSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	FDelegateHandle LevelTransitionHandle;

	TMap<FString, TArray<FSGUpgradePersistentData>> PersistentUpgradesByClass;

public:
	USGUpgradeSubsystem(){};
	
	FOnUpgradeCost OnUpgradeCost;
	FOnUpgradeLevel OnUpgradeLevel;
	FOnUpgrade OnUpgrade;
	FOnBindAttribute OnBindAttribute;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	/// @brief Registrera ett attribut som ska kunna uppgraderas
	/// @param Owner Ägaren av attributen (annars hittas den inte)
	/// @param PropertyName Vad attributen heter (måste just nu vara en float)
	/// @param RowName Raden i datatabellen som ska användas för uppgraderingarna
	/// @param Category Kategori för attributen (ex. "Player", "Enemy" osv)
	/// @param bFindOnReload
	void BindAttribute(UObject* Owner, FName PropertyName, FName RowName, FName Category, bool bFindOnReload = false);
	void BindDependentAttribute(UObject* Owner, FName PropertyName, bool OverrideOnModified, UObject* TargetOwner, FName TargetPropertyName);

	/// @brief Kalla på denna om en eller flera attribut som är länkade till samma radnamn ska uppgraderas. Alla uppgraderas utefter deras nuvarande nivå och synkas inte till samma.
	/// @param RowName Radnamnet i datatabellen
	void UpgradeByRow(FName RowName) const;

	void AttemptUpgrade(bool bUpgrade, const FName RowName, const FName Category) const;

	UFUNCTION(BlueprintCallable, Category="Upgrades")
	TArray<FSGUpgradeEntry> GetUpgradeEntries() const;

protected:
	FSGAttribute* GetByKey(UObject* Owner, FProperty* Property);
	const FSGAttribute* GetByCategory(FName Category, FName RowName) const;
	TArray<const FSGAttribute*> GetByRow(FName RowName) const;
	static uint64 GetKey(UObject* Owner, FProperty* Property);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Upgrades")
	UDataTable* UpgradeDataTable = nullptr;

	//Listan med alla attribut som ska kunna uppgraderas
	TArray<TUniquePtr<FSGAttribute>> RegisteredAttributes;

	//Listan med alla attribut som är beroende av andra attribut
	TArray<TUniquePtr<FSGDependentAttribute>> RegisteredDependentAttributes;

	//Ifall det ska finnas flera attribut som ska uppgraderas samtidigt (samma namn)
	TMap<FName, TArray<FSGAttribute*>> AttributesByRow;

	//För uppgradering av enstaka attribut
	TMap<uint64, FSGAttribute*> AttributesByKey;

	//För uppgradering av enstaka attribut
	TMap<FName, TArray<FSGAttribute*>> AttributesByCategory;

protected:
	void ClearAttributes(const FString& String);
	void SavePersistentUpgrades();
	FString GetClassNameKey(UObject* Object) const;

private:
	void BindDependentAttribute(UObject* Owner, FProperty* Prop, bool OverrideOnModified, FSGAttribute* TargetAttribute);

	FSGAUpgradeResult AttemptUpgrade(const FSGAttributeData& AttributeData, const FSGAttribute& TargetAttribute) const;
	void AnnounceUpgrade(const FSGAUpgradeResult& UpgradeResult) const;
	bool IsValidProperty(const FProperty* Property) const;

public:
	/// @brief Load all persistent upgrades from the previous level or from last play session. Used when loading save files.
	/// @param SavedAttributes containing all attributes to loaded and reconnected in the current level
	UFUNCTION(BlueprintCallable)
	void LoadSavedAttributes(const FSGSavedAttributes& SavedAttributes);

	/// @brief Save all persistent upgrades to be used in the next level or save file. Used when saving the game.
	/// @return FSGSavedAttributes containing all attributes to be saved and reconnected in the next level
	UFUNCTION(BlueprintCallable)
	FSGSavedAttributes GetSavedAttributes();
};
