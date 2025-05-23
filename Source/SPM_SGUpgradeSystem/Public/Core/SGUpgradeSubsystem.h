#pragma once

#include "CoreMinimal.h"
#include "Core/SGAttribute.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SGUpgradeSubsystem.generated.h"

//Spara attribut mellan levels
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

USTRUCT(Blueprintable)
struct FSGDependentUpgradePersistentData
{
	GENERATED_BODY()
	FString ClassNameKey;
	FName PropertyName;
	float InitialValue;
	bool bOverrideOnModified;
	FString TargetClassNameKey;
	FName TargetPropertyName;
	FSGUpgradePersistentData Dependency;
};

USTRUCT(Blueprintable)
struct FSGSavedAttributes
{
	GENERATED_BODY()
	TMap<FString, TArray<FSGUpgradePersistentData>> PersistentUpgradesByClass;
	TMap<FString, TArray<FSGDependentUpgradePersistentData>> PersistentDependenciesByClass;
};

struct FSGAUpgradeResult
{
	int32 Level;
	float Cost;
	bool bUpgraded;
};

//TODO: Snygga till. Flera för att undersöka vilka som lär vilja användas för UI osv
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpgradeFull, float, UpgradeLevel, float, UpgradeCost);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpgradeCost, float, UpgradeCost); 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpgradeLevel, float, UpgradeLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpgrade);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBindAttribute);

UCLASS()
class SPM_SGUPGRADESYSTEM_API USGUpgradeSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	FDelegateHandle LevelTransitionHandle;
	FDelegateHandle PostLevelLoadHandle;

	TMap<FString, TArray<FSGUpgradePersistentData>> PersistentUpgradesByClass;
	TMap<FString, TArray<FSGDependentUpgradePersistentData>> PersistentDependenciesByClass;
	FTimerHandle ValidationTimerHandle;

public:
	USGUpgradeSubsystem(){};
	
	FOnUpgradeFull OnUpgradeFull;
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
	//void BindAttribute(UObject* Owner, FName PropertyName, FName RowName, FName Category);

	void BindDependentAttribute(UObject* Owner, FName PropertyName, bool OverrideOnModified, UObject* TargetOwner, FName TargetPropertyName);

	/// @brief Kalla på denna om endast en unik attribut ska uppgraderas
	/// @param Owner Ägaren av attributen
	/// @param PropertyName Vad attributen heter 
	void ModifyAttribute(UObject* Owner, FName PropertyName) const;

	/// @brief Kalla på denna om en eller flera attribut som är länkade till samma radnamn ska uppgraderas. Alla uppgraderas utefter deras nuvarande nivå och synkas inte till samma.
	/// @param RowName Radnamnet i datatabellen
	void ModifyAttributeByRow(FName RowName) const;

	/// @brief Avregistrera ett attribut från att kunna uppgraderas
	/// @param Owner Ägaren av attributen
	/// @param PropertyName Vad attributen heter
	void UnbindAttribute(UObject* Owner, FName PropertyName);

	void RequestUpgrade(bool bUpgrade, UObject* Owner, FName PropertyName) const;

	void RequestUpgrade(bool bUpgrade, FName RowName) const;

	void RequestUpgrade(bool bUpgrade, const FName RowName, const FName Category) const;

	UFUNCTION(BlueprintCallable, Category="Upgrades")
	TArray<FSGUpgradeEntry> GetUpgradeEntries() const;

protected:
	const FSGAttribute* GetByKey(UObject* Owner, FProperty* Property) const;
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

	//För uppdatering av beroende attribut efter uppgradering
	TMap<uint64, TArray<FSGDependentAttribute*>> DependentAttributesByKey;

protected:
	void OnPreLevelChange(const FString& String);
	void SavePersistentUpgrades();

	void OnPostLevelChange(UWorld* World);
	void ReconnectAttributes();
	void ProcessObjectForReconnection(UObject* Object);
	void ValidateReferences();
	void RemoveAttributeFromCollections(const FSGAttribute* Attribute);

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
	void LoadPersistentUpgrades(const FSGSavedAttributes& SavedAttributes);

	/// @brief Save all persistent upgrades to be used in the next level or save file. Used when saving the game.
	/// @return FSGSavedAttributes containing all attributes to be saved and reconnected in the next level
	UFUNCTION(BlueprintCallable)
	FSGSavedAttributes GetPersistentUpgrades() const;
};
