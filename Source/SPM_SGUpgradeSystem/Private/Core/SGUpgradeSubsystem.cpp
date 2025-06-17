#include "Core/SGUpgradeSubsystem.h"

#include "Util/SGUpgradeDeveloperSettings.h"
#include "Engine/DataTable.h"
#include "UObject/UnrealType.h"

//TODO: Modify-funktionerna returnerar bool (för återanvändning från RequestUpgrade-funktionerna)?

void USGUpgradeSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	LevelTransitionHandle = FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &USGUpgradeSubsystem::ClearAttributes);

	const USGUpgradeDeveloperSettings* UpgradeSettings = GetDefault<USGUpgradeDeveloperSettings>();
	if (!UpgradeSettings)
	{
		return;
	}
	UpgradeDataTable = Cast<UDataTable>(UpgradeSettings->UpgradeDataTable.TryLoad());
}

void USGUpgradeSubsystem::Deinitialize()
{
	FCoreUObjectDelegates::PreLoadMap.Remove(LevelTransitionHandle);
	Super::Deinitialize();
}

void USGUpgradeSubsystem::ClearAttributes([[maybe_unused]] const FString& String)
{
	UE_LOG(LogTemp, Display, TEXT("💀USGUpgradeSubsystem::OnPreLevelChange: Clearing registered attributes..."));
	RegisteredAttributes.Empty();
	AttributesByRow.Empty();
	AttributesByKey.Empty();
	AttributesByCategory.Empty();
	RegisteredDependentAttributes.Empty();
}

void USGUpgradeSubsystem::SavePersistentUpgrades()
{
	if (RegisteredAttributes.Num() == 0)
	{
		return;
	}

	PersistentUpgradesByClass.Empty();

	for (const TUniquePtr<FSGAttribute>& AttributePtr : RegisteredAttributes)
	{
		const FSGAttribute* Attribute = AttributePtr.Get();
		if (!Attribute || !Attribute->Owner.IsValid())
		{
			continue;
		}

		UObject* Owner = Attribute->Owner.Get();
		if (!Owner || !IsValid(Owner))
		{
			continue;
		}
		FString ClassNameKey = GetClassNameKey(Owner);

		FSGUpgradePersistentData PersistentData;
		PersistentData.PropertyName = Attribute->Property->GetFName();
		PersistentData.RowName = Attribute->RowName;
		PersistentData.Category = Attribute->Category;
		PersistentData.CurrentUpgradeLevel = Attribute->CurrentUpgradeLevel;
		PersistentData.InitialValue = Attribute->InitialValue;

		PersistentUpgradesByClass.FindOrAdd(ClassNameKey).Add(PersistentData);
	}
}

void USGUpgradeSubsystem::LoadSavedAttributes(const FSGSavedAttributes& SavedAttributes)
{
	for (FSGSavedAttributeEntry SavedAttributeEntry : SavedAttributes.SomeAttributes)
	{
		PersistentUpgradesByClass.Add(SavedAttributeEntry.ClassNameKey, SavedAttributeEntry.PersistentUpgrades);

		FString key = SavedAttributeEntry.ClassNameKey;
		FSGAttribute* AttributeRaw = nullptr;
		for (const TUniquePtr<FSGAttribute>& RegisteredAttributePtr : RegisteredAttributes)		{

			FSGAttribute* RegisteredAttributeRaw = RegisteredAttributePtr.Get();
			if (!RegisteredAttributeRaw || !RegisteredAttributeRaw->Owner.IsValid())
			{
				continue;
			}

			UObject* Owner = RegisteredAttributeRaw->Owner.Get();
			if (!Owner || !IsValid(Owner))
			{
				continue;
			}

			FString ClassNameKey = GetClassNameKey(Owner);
			if (key.Compare(ClassNameKey) != 0)
			{
				continue;
			}

			AttributeRaw = RegisteredAttributeRaw;
			break;
		}

		if (!AttributeRaw)
		{
			continue;
		}

		for (FSGUpgradePersistentData PersistentUpgrade : SavedAttributeEntry.PersistentUpgrades)
		{
			if ( PersistentUpgrade.PropertyName.Compare(AttributeRaw->Property->GetFName()) != 0)
			{
				continue;
			}
			if (PersistentUpgrade.CurrentUpgradeLevel != AttributeRaw->CurrentUpgradeLevel && PersistentUpgrade.CurrentUpgradeLevel > 1)
			{
				for (int32 i = 0; i < PersistentUpgrade.CurrentUpgradeLevel - 1; ++i)
				{
					AttributeRaw->OnAttributeModified.Broadcast();
				}
			}
		}
	}
}

//------- BINDING

void USGUpgradeSubsystem::BindAttribute(UObject* Owner, const FName PropertyName, const FName RowName, const FName Category, const bool bFindOnReload)
{
	if (!Owner)
	{
		return;
	}

	TSubclassOf<UObject> OwnerClass = Owner->GetClass();
	UE_LOG(LogTemp, Display, TEXT("👉USGUpgradeSubsystem::BindAttribute: Owner Class: %s"),*OwnerClass->GetName());
	//Hämtar propertyn som ska bindas och gör early return om den inte finns/är giltig
	FProperty* Prop = Owner->GetClass()->FindPropertyByName(PropertyName);
	if (!IsValidProperty(Prop)) // Glöm inte att ändra denna om fler typer än float ska stödjas!
	{
		return;
	}

	if (const FSGAttribute* ExistingAttribute = GetByCategory(Category, RowName))
	{
		BindDependentAttribute(Owner, PropertyName, false, ExistingAttribute->Owner.Get(),ExistingAttribute->Property->GetFName());
		return;
	}

	uint64 Key = GetKey(Owner, Prop);

	//Hämtar propertyns data (för uppgradering) och gör early return om den inte finns
	const FSGAttributeData* AttributeData = UpgradeDataTable->FindRow<FSGAttributeData>(RowName, TEXT("BindAttribute"));
	if (!AttributeData)
	{
		return;
	}

	//Unik pekare för att säkerställa att inget blir knas vid eventuell omallokering och förflyttning av alla sparade attribut (vid ex utökning av listan).
	TUniquePtr<FSGAttribute> NewAttribute = MakeUnique<FSGAttribute>();
	NewAttribute->Owner = Owner;
	NewAttribute->Property = Prop;
	NewAttribute->RowName = RowName; //Vart man hittar den i datatabellen
	NewAttribute->InitialValue = CastFieldChecked<FFloatProperty>(Prop)->GetPropertyValue_InContainer(Owner);
	//Hämtar det initiala värdet från propertyn
	NewAttribute->Category = Category;
	NewAttribute->bSave = bFindOnReload;

	FSGAttribute* NewAttributeRaw = NewAttribute.Get();
	// För att inte riskera att tappa den i lambdan och behöva mecka
	NewAttributeRaw->OnAttributeModified.AddLambda([NewAttributeRaw, AttributeData]
	{
		//Early return om ägaren inte är aktiv och om uppgraderingen är maxad
		if (!NewAttributeRaw->Owner.IsValid())
		{
			return;
		}

		const FSGUpgradeData& UpgradeData = AttributeData->Data;
		if (UpgradeData.MaxNumberOfUpgrades <= NewAttributeRaw->CurrentUpgradeLevel && UpgradeData.MaxNumberOfUpgrades
			!= -1)
		{
			return;
		}
		NewAttributeRaw->CurrentUpgradeLevel++;

		//Hämta float-propertyn
		FFloatProperty* FloatProp = CastFieldChecked<FFloatProperty>(NewAttributeRaw->Property);
		float Current = FloatProp->GetPropertyValue_InContainer(NewAttributeRaw->Owner.Get());

		// Uppdaterar float-propertyn till den nya nivån
		Current += NewAttributeRaw->InitialValue * UpgradeData.Multiplier;
		FloatProp->SetPropertyValue_InContainer(NewAttributeRaw->Owner.Get(), Current);
	});

	UE_LOG(LogTemp, Display, TEXT("Binding Attribute: %s, Property: %s, RowName: %s, Category: %s"),
	       *NewAttributeRaw->RowName.ToString(), *NewAttributeRaw->Property->GetFName().ToString(),
	       *NewAttributeRaw->RowName.ToString(), *NewAttributeRaw->Category.ToString());
	// Lägg till i alla listor/loop-ups
	AttributesByRow.FindOrAdd(RowName).Add(NewAttributeRaw);
	AttributesByKey.Add(Key, NewAttributeRaw);
	AttributesByCategory.FindOrAdd(Category).Add(NewAttributeRaw);
	RegisteredAttributes.Add(MoveTemp(NewAttribute));
	OnBindAttribute.Broadcast();

	if (TArray<FSGUpgradePersistentData>* PersistentUpgrade = PersistentUpgradesByClass.Find(GetClassNameKey(Owner)))
	{
		UE_LOG(LogTemp, Display, TEXT("👉USGUpgradeSubsystem::BindAttribute: Found persistent upgrades for Owner: %s, Property: %s"),*Owner->GetName(), *Prop->GetFName().ToString());
		for (FSGUpgradePersistentData Upgrade : *PersistentUpgrade)
		{
			if (Upgrade.PropertyName.Compare(Prop->GetFName()) != 0)
			{
				continue;
			}
			if (Upgrade.CurrentUpgradeLevel == 1)
			{
				continue;
			}

			for (int32 i = 0; i < Upgrade.CurrentUpgradeLevel - 1; ++i)
			{
				NewAttributeRaw->OnAttributeModified.Broadcast();
				UE_LOG(LogTemp, Display,TEXT("👉USGUpgradeSubsystem::BindAttribute: Reconnecting Persistent Attribute"));
			}
			return;
		}
	}

}

void USGUpgradeSubsystem::BindDependentAttribute(UObject* Owner, const FName PropertyName, const bool OverrideOnModified,  UObject* TargetOwner, const FName TargetPropertyName)
{
	if (!Owner)
	{
		return;
	}

	FProperty* Prop = Owner->GetClass()->FindPropertyByName(PropertyName);
	if (!IsValidProperty(Prop)) // Update this if more types than float are supported!
	{
		return;
	}

	FSGAttribute* TargetAttribute = GetByKey(TargetOwner, TargetOwner->GetClass()->FindPropertyByName(TargetPropertyName));
	if (!TargetAttribute)
	{
		return;
	}

	BindDependentAttribute(Owner, Prop, OverrideOnModified, TargetAttribute);
}

void USGUpgradeSubsystem::BindDependentAttribute(UObject* Owner, FProperty* Prop, const bool OverrideOnModified, FSGAttribute* TargetAttribute)
{
	//Inga null-checkar av parametrar för de är gjordes i funktionerna: BindDependentAttribute (public) och BindAttribute.
	const FSGAttributeData* AttributeData = UpgradeDataTable->FindRow<FSGAttributeData>(
		TargetAttribute->RowName, TEXT("BindDependentAttribute"));
	if (!AttributeData)
	{
		return;
	}
	// Läggs på heapen för att säkerställa livscykeln
	TUniquePtr<FSGDependentAttribute> DependentAttribute = MakeUnique<FSGDependentAttribute>();
	DependentAttribute->Owner = Owner;
	DependentAttribute->Property = Prop;
	DependentAttribute->bOverrideOnModified = OverrideOnModified;

	//Rå pekare för att inte tappa den i lambdan
	FSGDependentAttribute* DependentAttributeRaw = DependentAttribute.Get();

	TargetAttribute->OnAttributeModified.AddLambda([TargetAttribute, AttributeData, DependentAttributeRaw]
	{
		// First check if both owner pointers are valid
		if (!DependentAttributeRaw->Owner.IsValid() || !TargetAttribute->Owner.IsValid())
		{
			return;
		}

		UObject* DepOwner = DependentAttributeRaw->Owner.Get();
		UObject* TargetOwner = TargetAttribute->Owner.Get();

		// Double check the owner objects are still valid
		if (!IsValid(DepOwner) || !IsValid(TargetOwner))
		{
			return;
		}
		const FFloatProperty* FloatProp = CastFieldChecked<const FFloatProperty>(DependentAttributeRaw->Property);
		float Current = FloatProp->GetPropertyValue_InContainer(DependentAttributeRaw->Owner.Get());

		if (DependentAttributeRaw->bOverrideOnModified)
		{
			const FFloatProperty* TargetFloatProp = CastFieldChecked<const FFloatProperty>(TargetAttribute->Property);
			float TargetsCurrent = TargetFloatProp->GetPropertyValue_InContainer(TargetAttribute->Owner.Get());
			Current = TargetsCurrent;
		}
		else
		{
			Current += TargetAttribute->InitialValue * AttributeData->Data.Multiplier;
		}
		FloatProp->SetPropertyValue_InContainer(DependentAttributeRaw->Owner.Get(), Current);
	});

	RegisteredDependentAttributes.Add(MoveTemp(DependentAttribute));
}

bool USGUpgradeSubsystem::IsValidProperty(const FProperty* Property) const
{
	return Property && Property->IsA<FFloatProperty>();
}


// ------- UPGRADING

void USGUpgradeSubsystem::UpgradeByRow(const FName RowName) const
{
	for (const FSGAttribute* TargetAttribute : GetByRow(RowName))
	{
		//Anropar lambdan som skapades vid bindandet.
		TargetAttribute->OnAttributeModified.Broadcast();
	}
}

void USGUpgradeSubsystem::AttemptUpgrade(const bool bUpgrade, const FName RowName, const FName Category) const
{
	if (!bUpgrade)
	{
		return;
	}
	const FSGAttribute* TargetAttribute = GetByCategory(Category, RowName);
	if (!TargetAttribute)
	{
		return;
	}

	//Hämtar propertyns data (för uppgradering) och gör early return om den inte finns
	const FSGAttributeData* AttributeData = UpgradeDataTable->FindRow<FSGAttributeData>(TargetAttribute->RowName, TEXT("RequestUpgrade"));
	if (!AttributeData)
	{
		return;
	}

	const FSGAUpgradeResult UpgradeResult = AttemptUpgrade(*AttributeData, *TargetAttribute);
	if (!UpgradeResult.bUpgraded)
	{
		return;
	}
	AnnounceUpgrade(UpgradeResult);
}

FSGAUpgradeResult USGUpgradeSubsystem::AttemptUpgrade(const FSGAttributeData& AttributeData, const FSGAttribute& TargetAttribute) const
{
	// Hämta uppgraderingsdata before it changes (updated after the Broadcast).
	const int32 LevelBeforeUpgrade = TargetAttribute.CurrentUpgradeLevel;
	const float UpgradeCost = AttributeData.Data.Cost * LevelBeforeUpgrade;

	// Call the lambda created during binding.
	TargetAttribute.OnAttributeModified.Broadcast();

	//Samla resultat (samlad plats för att modifiera vad som ska skickas ut)
	FSGAUpgradeResult Result;
	Result.Level = LevelBeforeUpgrade;
	Result.Cost = UpgradeCost;
	Result.bUpgraded = LevelBeforeUpgrade != TargetAttribute.CurrentUpgradeLevel;
	return Result;
}

void USGUpgradeSubsystem::AnnounceUpgrade(const FSGAUpgradeResult& UpgradeResult) const
{
	OnUpgradeCost.Broadcast(UpgradeResult.Cost);
	OnUpgrade.Broadcast();
}

// ------- GETTERS

TArray<FSGUpgradeEntry> USGUpgradeSubsystem::GetUpgradeEntries() const
{
	TArray<FSGUpgradeEntry> Out;
	if (!UpgradeDataTable)
	{
		return Out;
	}

	for (const TUniquePtr<FSGAttribute>& Ptr : RegisteredAttributes)
	{
		const FSGAttribute* TargetAttribute = Ptr.Get();

		if (!TargetAttribute || !TargetAttribute->Owner.IsValid() || TargetAttribute->Category == TEXT("Hidden"))
		{
			continue;
		}

		const FSGAttributeData* AttributeData = UpgradeDataTable->FindRow<FSGAttributeData>(TargetAttribute->RowName, TEXT("GetUpgradeEntries"));
		if (!AttributeData)
		{
			continue;
		}

		FFloatProperty* FloatProp = CastFieldChecked<FFloatProperty>(TargetAttribute->Property);
		const UObject* OwnerPtr = TargetAttribute->Owner.Get();
		if (!IsValid(OwnerPtr))
		{
			continue;
		}

		const float Current = FloatProp->GetPropertyValue_InContainer(OwnerPtr);

		FSGUpgradeEntry Entry;
		Entry.DisplayName = AttributeData->DisplayName;
		Entry.Icon = AttributeData->Icon.Get();
		Entry.Cost = AttributeData->Data.Cost * TargetAttribute->CurrentUpgradeLevel;
		Entry.Multiplier = TargetAttribute->InitialValue * AttributeData->Data.Multiplier;
		Entry.Category = TargetAttribute->Category;
		Entry.RowName = TargetAttribute->RowName;
		Entry.CurrentValue = Current;
		Entry.CurrentUpgradeLevel = TargetAttribute->CurrentUpgradeLevel;
		Entry.MaxNumberOfUpgrades = AttributeData->Data.MaxNumberOfUpgrades;
		Entry.DescriptionText = AttributeData->DescriptionText;
		Out.Add(Entry);
	}

	return Out;
}

FSGSavedAttributes USGUpgradeSubsystem::GetSavedAttributes()
{
	UE_LOG(LogTemp, Display, TEXT("👽USGUpgradeSubsystem::GetPersistentUpgrades: Saving persistent upgrades..."));
	SavePersistentUpgrades();
	FSGSavedAttributes SavedAttributes;
	for (const TPair UpgradesByClass : PersistentUpgradesByClass)
	{
		FString ClassNameKey = UpgradesByClass.Key;
		FSGSavedAttributeEntry SavedEntry;
		SavedEntry.ClassNameKey = ClassNameKey;
		SavedEntry.PersistentUpgrades = UpgradesByClass.Value;

		SavedAttributes.SomeAttributes.Add(SavedEntry);
		UE_LOG(LogTemp, Display, TEXT("👽LoadPersistentUpgrades: ClassNameKey = %s, and PersistentUpgrades: %d"),*ClassNameKey, UpgradesByClass.Value.Num());
	}
	UE_LOG(LogTemp, Display, TEXT("👽LoadPersistentUpgrades: Saving PersistentUpgrades: Orbs = %f"),SavedAttributes.Orbs);

	return SavedAttributes;
}

// ------- HELPER GETTERS

FSGAttribute* USGUpgradeSubsystem::GetByKey(UObject* Owner, FProperty* Property)
{
	return AttributesByKey.FindRef(GetKey(Owner, Property));
}

const FSGAttribute* USGUpgradeSubsystem::GetByCategory(const FName Category, FName RowName) const
{
	const TArray<FSGAttribute*>* Attributes = AttributesByCategory.Find(Category);
	if (!Attributes)
	{
		return nullptr;
	}

	FSGAttribute* const* TargetAttribute = Attributes->FindByPredicate([RowName](const FSGAttribute* Attri)
	{
		return Attri->RowName == RowName;
	});

	return TargetAttribute ? *TargetAttribute : nullptr;
}

TArray<const FSGAttribute*> USGUpgradeSubsystem::GetByRow(FName RowName) const
{
	TArray<const FSGAttribute*> Out;
	if (const TArray<FSGAttribute*>* Arr = AttributesByRow.Find(RowName))
	{
		for (const FSGAttribute* Mod : *Arr)
		{
			Out.Add(Mod);
		}
	}
	return Out;
}

uint64 USGUpgradeSubsystem::GetKey(UObject* Owner, FProperty* Property)
{
	/* Här leks det med bitar...
	 * Slår ihop adressen för Owner respektive Property till en nyckel. Lite hackig lösning för att se till att nyckeln blir unik.
	 * Kan bli knas om själva adresserna har flyttats och omallokerats i minnet - gäller då mest dynamiskt skapade actors och komponenter.
	 * Men Actors som skapas vid start (typ spelaren) har samma adress under hela livscykeln, så det borde vara lugnt👀️
	 */
	//TODO: ...Kanske borde ändra till hashing istället?
	return reinterpret_cast<uint64>(Owner) << 32 ^ reinterpret_cast<uint64>(Property);
}

FString USGUpgradeSubsystem::GetClassNameKey(UObject* Object) const
{
	return FString::Printf(TEXT("%s_%s"), *Object->GetClass()->GetName(), *Object->GetName());
}

// Unused functions

/*
void USGUpgradeSubsystem::UnbindAttribute(UObject* Owner, const FName PropertyName)
{ //TODO: Se över om denna funkar helt korrekt fortfarande. Inte ordentligt kollad efter tillägg av kategori.
	if (!ensureMsgf(Owner, TEXT("Owner was nullptr")))
	{
		return;
	}

	//Hämtar
	FProperty* TargetProperty = Owner->GetClass()->FindPropertyByName(PropertyName);
	uint64 Key = GetKey(Owner, TargetProperty);
	FSGAttribute* TargetAttribute = AttributesByKey.FindRef(Key);
	if (!ensureMsgf(TargetAttribute, TEXT("TargetAttribute was nullptr")))
	{
		return;
	}

	//Tar bort
	if (TArray<FSGAttribute*>* Arr = AttributesByRow.Find(TargetAttribute->RowName))
	{
		Arr->Remove(TargetAttribute);
	}
	AttributesByKey.Remove(Key);
	RegisteredAttributes.RemoveAll([TargetAttribute](const TUniquePtr<FSGAttribute>& Ptr)
	{
		return Ptr.Get() == TargetAttribute;
	});
	AttributesByCategory.Remove(TargetAttribute->Category);
}*/

/*
void USGUpgradeSubsystem::ModifyAttribute(UObject* Owner, FName PropertyName) const
{
	if (!Owner)
	{
		return;
	}

	FProperty* Prop = Owner->GetClass()->FindPropertyByName(PropertyName);
	const FSGAttribute* TargetAttribute = GetByKey(Owner, Prop);
	if (!TargetAttribute)
	{
		return;
	}

	//Anropar lambdan som skapades vid bindandet.
	TargetAttribute->OnAttributeModified.Broadcast();
}
*/

/*
void USGUpgradeSubsystem::RequestUpgrade(const bool bUpgrade, UObject* Owner, const FName PropertyName) const
{
	if (!Owner || !bUpgrade)
	{
		return;
	}

	FProperty* Prop = Owner->GetClass()->FindPropertyByName(PropertyName);
	const FSGAttribute* TargetAttribute = GetByKey(Owner, Prop);
	if (!TargetAttribute)
	{
		return;
	}

	//Hämtar propertyns data (för uppgradering) och gör early return om den inte finns
	const FSGAttributeData* AttributeData = UpgradeDataTable->FindRow<FSGAttributeData>(
		TargetAttribute->RowName, TEXT("RequestUpgrade"));
	if (!AttributeData)
	{
		return;
	}

	const FSGAUpgradeResult UpgradeResult = AttemptUpgrade(*AttributeData, *TargetAttribute);
	if (!UpgradeResult.bUpgraded)
	{
		return;
	}
	AnnounceUpgrade(UpgradeResult);
}
*/

/*void USGUpgradeSubsystem::RequestUpgrade(const bool bUpgrade, const FName RowName) const
{
	if (!bUpgrade)
	{
		return;
	}

	//Hämtar data för angiven rad (för uppgradering) och gör early return om den inte finns
	const FSGAttributeData* AttributeData = UpgradeDataTable->FindRow<
		FSGAttributeData>(RowName, TEXT("RequestUpgrade"));
	if (!AttributeData)
	{
		return;
	}

	for (const FSGAttribute* TargetAttribute : GetByRow(RowName))
	{
		const FSGAUpgradeResult UpgradeData = AttemptUpgrade(*AttributeData, *TargetAttribute);
		if (!UpgradeData.bUpgraded)
		{
			return;
		}
		AnnounceUpgrade(UpgradeData);
	}
}*/

/*const FSGAttribute* USGUpgradeSubsystem::GetByKey(UObject* Owner, FProperty* Property) const
{
	FSGAttribute* const* Found = AttributesByKey.Find(GetKey(Owner, Property));
	return Found ? *Found : nullptr;
}*/
