#include "Core/SGUpgradeSubsystem.h"
#include "Util/SGUpgradeDeveloperSettings.h"
#include "Engine/DataTable.h"
#include "UObject/UnrealType.h"

//TODO: Se över om fler hjälpfunktioner kan öka läsbarhet och minska kodupprepning (gäller framförallt Modify-& RequestUpgrade-funktionerna)... Varför heter inte alla nåt med upgrade ba?
//TODO: Modify-funktionerna returnerar bool (för återanvändning från RequestUpgrade-funktionerna)?

void USGUpgradeSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	//Hämta datatabellen som ska användas för uppgraderingarna
	const USGUpgradeDeveloperSettings* UpgradeSettings = GetDefault<USGUpgradeDeveloperSettings>();
	if (!UpgradeSettings)
	{
		return;
	}
	UpgradeDataTable = Cast<UDataTable>(UpgradeSettings->UpgradeDataTable.TryLoad());
}

//------- BIND

void USGUpgradeSubsystem::BindAttribute(UObject* Owner, const FName PropertyName, const FName RowName, const FName Category)
{
	if (!ensureMsgf(Owner, TEXT("Owner was nullptr")) || !ensureMsgf(UpgradeDataTable,  TEXT("UpgradeDataTable was nullptr")))
	{
		return;
	}

	//Hämtar propertyn som ska bindas och gör early return om den inte finns/är giltig
	FProperty* Prop = Owner->GetClass()->FindPropertyByName(PropertyName);
	if (!Prop || !Prop->IsA<FFloatProperty>()) // Glöm inte att ändra denna om fler typer än float ska stödjas!
	{
		UE_LOG(LogTemp, Error, TEXT("Property %s not found or not a float property"), *PropertyName.ToString());
		return;
	}

	if (const FSGAttribute* FoundExisting = GetByCategory(Category, RowName))
	{
		FoundExisting->Category = Category; //TODO: Se över alternativ. Snabblösning för att fungera med arvshierarkin för vapen. (Basklassen Gun's BeginPlay-funktion körs före.)
		return;
	}

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
	NewAttribute->InitialValue = CastFieldChecked<FFloatProperty>(Prop)->GetPropertyValue_InContainer(Owner); //Hämtar det initiala värdet från propertyn
	NewAttribute->Category = Category;

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
		if (UpgradeData.MaxNumberOfUpgrades <= NewAttributeRaw->CurrentUpgradeLevel && UpgradeData.MaxNumberOfUpgrades != -1)
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
		UE_LOG(LogTemp, Error, TEXT("Current %f"), Current);
	});

	// Lägg till i alla listor/loop-ups
	AttributesByRow.FindOrAdd(RowName).Add(NewAttributeRaw);
	AttributesByKey.Add(GetKey(Owner, Prop), NewAttributeRaw);
	AttributesByCategory.FindOrAdd(Category).Add(NewAttributeRaw);
	RegisteredAttributes.Add(MoveTemp(NewAttribute));
	OnBindAttribute.Broadcast();
}

//------- HELPERS

const FSGAttribute* USGUpgradeSubsystem::GetByKey(UObject* Owner, FProperty* Property) const
{
	FSGAttribute* const* Found = AttributesByKey.Find(GetKey(Owner, Property));
	return Found ? *Found : nullptr;
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

void USGUpgradeSubsystem::ModifyAttribute(UObject* Owner, FName PropertyName) const
{
	if (!ensureMsgf(Owner, TEXT("Owner was nullptr")))
	{
		return;
	}

	FProperty* Prop = Owner->GetClass()->FindPropertyByName(PropertyName);
	const FSGAttribute* TargetAttribute = GetByKey(Owner, Prop);
	if (!ensureMsgf(TargetAttribute, TEXT("TargetAttribute was nullptr")))
	{
		return;
	}

	//Anropar lambdan som skapades vid bindandet.
	TargetAttribute->OnAttributeModified.Broadcast();
}

void USGUpgradeSubsystem::ModifyAttributeByRow(FName RowName) const
{
	for (const FSGAttribute* TargetAttribute : GetByRow(RowName))
	{
		//Anropar lambdan som skapades vid bindandet.
		TargetAttribute->OnAttributeModified.Broadcast();
	}
}

//TODO: Se över om denna funkar helt korrekt fortfarande. Inte ordentligt kollad efter tillägg av kategori.
void USGUpgradeSubsystem::UnbindAttribute(UObject* Owner, const FName PropertyName)
{
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
}

void USGUpgradeSubsystem::RequestUpgrade(const bool bUpgrade, UObject* Owner, const FName PropertyName) const
{
	if (!Owner || !bUpgrade)
	{
		return;
	}

	FProperty* Prop = Owner->GetClass()->FindPropertyByName(PropertyName);
	const FSGAttribute* TargetAttribute = GetByKey(Owner, Prop);
	if (!ensureMsgf(TargetAttribute, TEXT("TargetAttribute was nullptr")))
	{
		return;
	}

	const int32 PreviousUpgradeLevel = TargetAttribute->CurrentUpgradeLevel;
	TargetAttribute->OnAttributeModified.Broadcast();
	// Early return om attributen inte blev uppgraderad
	if (PreviousUpgradeLevel == TargetAttribute->CurrentUpgradeLevel)
	{
		return;
	}

	//Hämtar propertyns data (för uppgradering) och gör early return om den inte finns
	const FSGAttributeData* AttributeData = UpgradeDataTable->FindRow<FSGAttributeData>(TargetAttribute->RowName, TEXT("RequestUpgrade"));
	if (!AttributeData)
	{
		return;
	}

	const float UpgradeCost = AttributeData->Data.Cost * TargetAttribute->CurrentUpgradeLevel;
	OnUpgradeFull.Broadcast(TargetAttribute->CurrentUpgradeLevel, UpgradeCost);
	OnUpgradeCost.Broadcast(UpgradeCost);
	OnUpgradeLevel.Broadcast(TargetAttribute->CurrentUpgradeLevel);
	OnUpgrade.Broadcast();
}

void USGUpgradeSubsystem::RequestUpgrade(const bool bUpgrade, const FName RowName) const
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
		const int32 LevelBeforeUpgrade = TargetAttribute->CurrentUpgradeLevel;

		//Anropar lambdan som skapades vid bindandet.
		TargetAttribute->OnAttributeModified.Broadcast();

		// Early return om attributen inte blev uppgraderad
		if (LevelBeforeUpgrade == TargetAttribute->CurrentUpgradeLevel)
		{
			return;
		}
		//Hämtar uppgraderingsdatan för den nya nivån (har uppdaterats efter Broadcasten).
		const FSGUpgradeData& UpgradeData = AttributeData->Data;

		OnUpgradeFull.Broadcast(TargetAttribute->CurrentUpgradeLevel, UpgradeData.Cost);
		OnUpgradeCost.Broadcast(UpgradeData.Cost);
		OnUpgradeLevel.Broadcast(TargetAttribute->CurrentUpgradeLevel);
		OnUpgrade.Broadcast();
	}
}

//------- UI
void USGUpgradeSubsystem::RequestUpgrade(const bool bUpgrade, const FName RowName, const FName Category) const
{
	const FSGAttribute* TargetAttribute = GetByCategory(Category, RowName);
	if (!TargetAttribute)
	{
		return;
	}
	if (!bUpgrade)
	{
		return;
	}
	
	const int32 LevelBeforeUpgrade = TargetAttribute->CurrentUpgradeLevel;
	//Anropar lambdan som skapades vid bindandet.
	TargetAttribute->OnAttributeModified.Broadcast();

	// Early return om attributen inte blev uppgraderad
	if (LevelBeforeUpgrade == TargetAttribute->CurrentUpgradeLevel)
	{
		return;
	}

	//Hämtar propertyns data (för uppgradering) och gör early return om den inte finns
	const FSGAttributeData* AttributeData = UpgradeDataTable->FindRow<FSGAttributeData>(TargetAttribute->RowName, TEXT("RequestUpgrade"));
	if (!AttributeData)
	{
		return;
	}

	//Hämtar uppgraderingsdatan för den nya nivån (har uppdaterats efter Broadcasten).
	const FSGUpgradeData& UpgradeData = AttributeData->Data;

	OnUpgradeFull.Broadcast(TargetAttribute->CurrentUpgradeLevel, UpgradeData.Cost);
	OnUpgradeCost.Broadcast(UpgradeData.Cost);
	OnUpgradeLevel.Broadcast(TargetAttribute->CurrentUpgradeLevel);
	OnUpgrade.Broadcast();

}

TArray<FSGUpgradeEntry> USGUpgradeSubsystem::GetUpgradeEntries() const
{
	TArray<FSGUpgradeEntry> Out;
	if (!ensureMsgf(UpgradeDataTable, TEXT("UpgradeDataTable was nullptr")))
	{
		return Out;
	}

	for (const TUniquePtr<FSGAttribute>& Ptr : RegisteredAttributes)
	{
		const FSGAttribute* TargetAttribute = Ptr.Get();
		const FSGAttributeData* AttributeData = UpgradeDataTable->FindRow<FSGAttributeData>(TargetAttribute->RowName, TEXT("GetUpgradeEntries"));
		if (!ensureMsgf(AttributeData, TEXT("AttributeData was nullptr")))
		{
			continue;
		}
	
		FFloatProperty* FloatProp = CastFieldChecked<FFloatProperty>(TargetAttribute->Property);
		const float Current = FloatProp->GetPropertyValue_InContainer(TargetAttribute->Owner.Get());
		
		FSGUpgradeEntry Entry;
		Entry.DisplayName = AttributeData->DisplayName;
		Entry.Icon = AttributeData->Icon.Get();
		Entry.Cost = AttributeData->Data.Cost * TargetAttribute->CurrentUpgradeLevel;
		Entry.Multiplier = AttributeData->Data.Multiplier * 100;
		Entry.Category = TargetAttribute->Category;
		Entry.RowName = TargetAttribute->RowName;
		Entry.CurrentValue = Current; //TODO: centrera - samma kalkylering sker på två ställen		
		Entry.CurrentUpgradeLevel = TargetAttribute->CurrentUpgradeLevel;
		Entry.MaxNumberOfUpgrades = AttributeData->Data.MaxNumberOfUpgrades;

		Out.Add(Entry);
	}

	return Out;
}
