#include "Core/SGUpgradeSubsystem.h"

#include "EngineUtils.h"
#include "Util/SGUpgradeDeveloperSettings.h"
#include "Engine/DataTable.h"
#include "UObject/UnrealType.h"

//TODO: Se över om fler hjälpfunktioner kan öka läsbarhet och minska kodupprepning (gäller framförallt Modify-& RequestUpgrade-funktionerna)... Varför heter inte alla nåt med upgrade ba?
//TODO: Modify-funktionerna returnerar bool (för återanvändning från RequestUpgrade-funktionerna)?

void USGUpgradeSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	LevelTransitionHandle = FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &USGUpgradeSubsystem::OnPreLevelChange);
	PostLevelLoadHandle = FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(
		this, &USGUpgradeSubsystem::OnPostLevelChange);

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
	FCoreUObjectDelegates::PostLoadMapWithWorld.Remove(PostLevelLoadHandle);

	Super::Deinitialize();
}

void USGUpgradeSubsystem::OnPreLevelChange([[maybe_unused]] const FString& String)
{
	SavePersistentUpgrades();

	RegisteredAttributes.Empty();
	AttributesByRow.Empty();
	AttributesByKey.Empty();
	AttributesByCategory.Empty();
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
		if (!Attribute || !Attribute->Owner.IsValid() || !Attribute->bFindOnReload)
		{
			continue;
		}

		UObject* Owner = Attribute->Owner.Get();
		FString ClassNameKey = GetClassNameKey(Owner);

		FSGUpgradePersistentData PersistentData;
		PersistentData.PropertyName = Attribute->Property->GetFName();
		PersistentData.RowName = Attribute->RowName;
		PersistentData.Category = Attribute->Category;
		PersistentData.CurrentUpgradeLevel = Attribute->CurrentUpgradeLevel;
		PersistentData.InitialValue = Attribute->InitialValue;

		PersistentUpgradesByClass.FindOrAdd(ClassNameKey).Add(PersistentData);
		int32 Key = GetKey(Owner, Attribute->Property);
		TArray<FSGDependentAttribute*>* Dependencies = DependentAttributesByKey.Find(Key);
		if (!Dependencies)
		{
			continue;
		}
		for (FSGDependentAttribute* Dependency : *Dependencies)
		{
			FSGDependentUpgradePersistentData PersistentDependentData;
			PersistentDependentData.ClassNameKey = GetClassNameKey(Dependency->Owner.Get());
			PersistentDependentData.PropertyName = Dependency->Property->GetFName();
			PersistentDependentData.bOverrideOnModified = Dependency->bOverrideOnModified;
			PersistentDependentData.InitialValue = CastFieldChecked<FFloatProperty>(Dependency->Property)->
				GetPropertyValue_InContainer(Dependency->Owner.Get());
			PersistentDependentData.TargetClassNameKey = GetClassNameKey(Dependency->Owner.Get());
			PersistentDependentData.TargetPropertyName = Dependency->Property->GetFName();

			FString DependencyClassNameKey = GetClassNameKey(Dependency->Owner.Get());
			PersistentDependenciesByClass.FindOrAdd(DependencyClassNameKey).Add(PersistentDependentData);
		}
	}
}

void USGUpgradeSubsystem::LoadPersistentUpgrades(const FSGSavedAttributes& SavedAttributes)
{
	PersistentUpgradesByClass = SavedAttributes.PersistentUpgradesByClass;
	PersistentDependenciesByClass = SavedAttributes.PersistentDependenciesByClass;
	/*ReconnectAttributes();
	ValidateReferences();*/
}

FSGSavedAttributes USGUpgradeSubsystem::GetPersistentUpgrades() const
{
	FSGSavedAttributes SavedAttributes;
	SavedAttributes.PersistentUpgradesByClass = PersistentUpgradesByClass;
	SavedAttributes.PersistentDependenciesByClass = PersistentDependenciesByClass;
	return SavedAttributes;
}

void USGUpgradeSubsystem::OnPostLevelChange(UWorld* World)
{
	// Behövs inte då alla attribut ska återställas vid restart och alla Actors i objektpoolen skapas på nytt
	//TODO: Implementera att objektpoolen behåller actors vid Restart/GameOver (utan att de städas bort av garbage collection. Tidigare försök orsakade krash).
	/*FTimerHandle ReconnectTimerHandle;
	World->GetTimerManager().SetTimer(ReconnectTimerHandle, FTimerDelegate::CreateUObject(this, &USGUpgradeSubsystem::ReconnectAttributes),0.5f, false);
	World->GetTimerManager().SetTimer(ValidationTimerHandle,this, &USGUpgradeSubsystem::ValidateReferences, 10.0f, true);*/
}

void USGUpgradeSubsystem::ReconnectAttributes()
{
	if (!GetWorld())
	{
		return;
	}

	TArray<UObject*> AllObjects;

	// TODO: Testa om detta även går med fiender från object poolen
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		ProcessObjectForReconnection(Actor);

		TArray<UActorComponent*> Components;
		Actor->GetComponents(Components);

		for (UActorComponent* Component : Components)
		{
			ProcessObjectForReconnection(Component);
		}
	}
}

void USGUpgradeSubsystem::ProcessObjectForReconnection(UObject* Object)
{
	if (!Object || !IsValid(Object))
	{
		return;
	}

	FString ClassNameKey = GetClassNameKey(Object);
	if (TArray<FSGUpgradePersistentData>* StoredData = PersistentUpgradesByClass.Find(ClassNameKey))
	{
		for (const FSGUpgradePersistentData& Data : *StoredData)
		{
			FProperty* Prop = Object->GetClass()->FindPropertyByName(Data.PropertyName);
			if (!IsValidProperty(Prop))
			{
				continue;
			}

			BindAttribute(Object, Data.PropertyName, Data.RowName, Data.Category, true);

			uint64 Key = GetKey(Object, Prop);
			if (FSGAttribute** FoundAttr = AttributesByKey.Find(Key))
			{
				FSGAttribute* NewAttribute = *FoundAttr;

				NewAttribute->InitialValue = Data.InitialValue;

				//Bortkommenterat så alla attribut reset:as när leveln startas om
				/*if (Data.CurrentUpgradeLevel == 1)
				{
					continue;
				}
				for (int32 i = 0; i < Data.CurrentUpgradeLevel-1; ++i)
				{
					NewAttribute->OnAttributeModified.Broadcast();
				}*/
			}
		}
	}
	else if (TArray<FSGDependentUpgradePersistentData>* StoredDependentData = PersistentDependenciesByClass.
		Find(ClassNameKey))
	{
		for (const FSGDependentUpgradePersistentData& Data : *StoredDependentData)
		{
			UObject* TargetObject = StaticFindObject(UObject::StaticClass(), nullptr, *Data.ClassNameKey);
			if (!TargetObject)
			{
				continue;
			}

			FProperty* Prop = TargetObject->GetClass()->FindPropertyByName(Data.TargetPropertyName);
			if (!IsValidProperty(Prop))
			{
				continue;
			}
			BindDependentAttribute(Object, Data.PropertyName, Data.bOverrideOnModified, TargetObject,
			                       Data.TargetPropertyName);
		}
	}
}

void USGUpgradeSubsystem::ValidateReferences()
{
	for (auto It = RegisteredAttributes.CreateIterator(); It; ++It)
	{
		FSGAttribute* Attribute = It->Get();
		if (!Attribute || !Attribute->Owner.IsValid() || !IsValid(Attribute->Owner.Get()))
		{
			RemoveAttributeFromCollections(Attribute);
			It.RemoveCurrent();
		}
	}

	for (auto It = RegisteredDependentAttributes.CreateIterator(); It; ++It)
	{
		FSGDependentAttribute* Attribute = It->Get();
		if (!Attribute || !Attribute->Owner.IsValid() || !IsValid(Attribute->Owner.Get()))
		{
			uint64 Key = GetKey(Attribute->Owner.Get(), Attribute->Property);
			DependentAttributesByKey.Remove(Key);
			It.RemoveCurrent();
		}
	}
}

void USGUpgradeSubsystem::RemoveAttributeFromCollections(const FSGAttribute* Attribute)
{
	if (!Attribute)
	{
		return;
	}

	if (TArray<FSGAttribute*>* Arr = AttributesByRow.Find(Attribute->RowName))
	{
		Arr->Remove(const_cast<FSGAttribute*>(Attribute));
	}

	if (TArray<FSGAttribute*>* Arr = AttributesByCategory.Find(Attribute->Category))
	{
		Arr->Remove(const_cast<FSGAttribute*>(Attribute));
	}

	if (Attribute->Owner.IsValid())
	{
		uint64 Key = GetKey(Attribute->Owner.Get(), Attribute->Property);
		AttributesByKey.Remove(Key);
	}
}

FString USGUpgradeSubsystem::GetClassNameKey(UObject* Object) const
{
	return FString::Printf(TEXT("%s_%s"), *Object->GetClass()->GetName(), *Object->GetName());
}

//------- BIND

void USGUpgradeSubsystem::BindAttribute(UObject* Owner, const FName PropertyName, const FName RowName, FName Category, bool bFindOnReload)
{
	if (!Owner)
	{
		return;
	}

	//Hämtar propertyn som ska bindas och gör early return om den inte finns/är giltig
	FProperty* Prop = Owner->GetClass()->FindPropertyByName(PropertyName);
	if (!IsValidProperty(Prop)) // Glöm inte att ändra denna om fler typer än float ska stödjas!
	{
		return;
	}

	if (const FSGAttribute* ExistingAttribute = GetByCategory(Category, RowName))
	{
		BindDependentAttribute(Owner, PropertyName, false, ExistingAttribute->Owner.Get(),
		                       ExistingAttribute->Property->GetFName());
		return;
		//Category = TEXT("Hidden");
		//FoundExisting->Category = Category; //TODO: Se över alternativ. Snabblösning för att fungera med arvshierarkin för vapen. (Basklassen Gun's BeginPlay-funktion körs före.)
		//return;
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
	NewAttribute->InitialValue = CastFieldChecked<FFloatProperty>(Prop)->GetPropertyValue_InContainer(Owner);
	//Hämtar det initiala värdet från propertyn
	NewAttribute->Category = Category;
	NewAttribute->bFindOnReload = bFindOnReload;

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

	// Lägg till i alla listor/loop-ups
	AttributesByRow.FindOrAdd(RowName).Add(NewAttributeRaw);
	AttributesByKey.Add(GetKey(Owner, Prop), NewAttributeRaw);
	AttributesByCategory.FindOrAdd(Category).Add(NewAttributeRaw);
	RegisteredAttributes.Add(MoveTemp(NewAttribute));
	OnBindAttribute.Broadcast();
}

void USGUpgradeSubsystem::BindDependentAttribute(UObject* Owner, FName PropertyName, const bool OverrideOnModified, UObject* TargetOwner, FName TargetPropertyName)
{
	if (!Owner)
	{
		return;
	}

	//Hämtar propertyn som ska bindas och gör early return om den inte finns/är giltig
	FProperty* Prop = Owner->GetClass()->FindPropertyByName(PropertyName);
	if (!IsValidProperty(Prop)) // Update this if more types than float are supported!
	{
		return;
	}

	FSGAttribute* TargetAttribute = GetByKey(TargetOwner,
	                                         TargetOwner->GetClass()->FindPropertyByName(TargetPropertyName));
	if (!TargetAttribute)
	{
		return;
	}

	BindDependentAttribute(Owner, Prop, OverrideOnModified, TargetAttribute);
}

//------- HELPERS

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
	int32 Key = GetKey(DependentAttributeRaw->Owner.Get(), DependentAttributeRaw->Property);
	DependentAttributesByKey.FindOrAdd(Key).Add(DependentAttributeRaw);
	RegisteredDependentAttributes.Add(MoveTemp(DependentAttribute));
}

const FSGAttribute* USGUpgradeSubsystem::GetByKey(UObject* Owner, FProperty* Property) const
{
	FSGAttribute* const* Found = AttributesByKey.Find(GetKey(Owner, Property));
	return Found ? *Found : nullptr;
}

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
		const FSGAUpgradeResult UpgradeData = AttemptUpgrade(*AttributeData, *TargetAttribute);
		if (!UpgradeData.bUpgraded)
		{
			return;
		}
		AnnounceUpgrade(UpgradeData);
	}
}

//------- UI
void USGUpgradeSubsystem::RequestUpgrade(const bool bUpgrade, const FName RowName, const FName Category) const
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

		const FSGAttributeData* AttributeData = UpgradeDataTable->FindRow<FSGAttributeData>(
			TargetAttribute->RowName, TEXT("GetUpgradeEntries"));
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
		//AttributeData->Data.Multiplier * 100;
		Entry.Category = TargetAttribute->Category;
		Entry.RowName = TargetAttribute->RowName;
		Entry.CurrentValue = Current;
		Entry.CurrentUpgradeLevel = TargetAttribute->CurrentUpgradeLevel;
		Entry.MaxNumberOfUpgrades = AttributeData->Data.MaxNumberOfUpgrades;

		Out.Add(Entry);
	}

	return Out;
}


//------ UPGRADE HELPERS
FSGAUpgradeResult USGUpgradeSubsystem::AttemptUpgrade(const FSGAttributeData& AttributeData,
                                                      const FSGAttribute& TargetAttribute) const
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
	//TODO: Ta bort oanvända delegates
	OnUpgradeFull.Broadcast(UpgradeResult.Level, UpgradeResult.Cost);
	OnUpgradeCost.Broadcast(UpgradeResult.Cost);
	OnUpgradeLevel.Broadcast(UpgradeResult.Level);
	OnUpgrade.Broadcast();
}

bool USGUpgradeSubsystem::IsValidProperty(const FProperty* Property) const
{
	return Property && Property->IsA<FFloatProperty>();
}
