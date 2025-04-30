#pragma once

#include "CoreMinimal.h"
#include "Core/SGAttribute.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SGUpgradeSubsystem.generated.h"

//TODO: Snygga till. Flera för att undersöka vilka som lär vilja användas för UI osv
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpgradeFull, float, UpgradeLevel, float, UpgradeCost);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpgradeCost, float, UpgradeCost); 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpgradeLevel, float, UpgradeLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpgrade);

UCLASS()
class SPM_SGUPGRADESYSTEM_API USGUpgradeSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	USGUpgradeSubsystem(){};
	
	FOnUpgradeFull OnUpgradeFull;
	FOnUpgradeCost OnUpgradeCost;
	FOnUpgradeLevel OnUpgradeLevel;
	FOnUpgrade OnUpgrade;
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/// @brief Registrera ett attribut som ska kunna uppgraderas
	/// @param Owner Ägaren av attributen (annars hittas den inte)
	/// @param PropertyName Vad attributen heter (måste just nu vara en float)
	/// @param RowName Raden i datatabellen som ska användas för uppgraderingarna
	void BindAttribute(UObject* Owner, FName PropertyName, FName RowName);

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
protected:
	
	const FSGAttribute* GetByKey(UObject* Owner, FProperty* Property) const;
	
	TArray<const FSGAttribute*> GetByRow(FName RowName) const;

	uint64 GetKey(UObject* Owner, FProperty* Property) const;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Upgrades")
	UDataTable* UpgradeDataTable = nullptr;

	//Listan med alla attribut som ska kunna uppgraderas
	TArray<TUniquePtr<FSGAttribute>> RegisteredAttributes;

	//Ifall det ska finnas flera attribut som ska uppgraderas samtidigt (samma namn) 
	TMap<FName, TArray<FSGAttribute*>> AttributesByRow;

	//För uppgradering av enstaka attribut
	TMap<uint64, FSGAttribute*> AttributesByKey;
};
