// Joel Larsson Wendt | jola6902

#include "jola6902_GunsComponent.h"
#include "SGWeaponsHUD.h"
#include "Blueprint/UserWidget.h"
#include "Gear/Weapons/SGGun.h"

// Public
Ujola6902_GunsComponent::Ujola6902_GunsComponent(): Owner(GetOwner())
{
	PrimaryComponentTick.bCanEverTick = true;
}

void Ujola6902_GunsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	UpdateGunsHUD();
}

// Protected
void Ujola6902_GunsComponent::BeginPlay()
{
	Super::BeginPlay();

	CreateGunsHUD();
	SetUpGuns();
}

// Public
void Ujola6902_GunsComponent::FireGun()
{
	//UE_LOG(LogTemp, Warning, TEXT("Trying to fire gun at index: %d"), CurrentGunIndex);
	ASGGun* Gun = Guns[CurrentGunIndex];
	if (Gun)
	{
		if (Gun->GetAmmoClip() <= 0)
		{
			ReloadGun();
		}
		
		Gun->Fire();
	}
}

void Ujola6902_GunsComponent::ReloadGun()
{
	UE_LOG(LogTemp, Warning, TEXT("SGPlayerCharacter::ReloadGun()"));
	
	if (Guns[CurrentGunIndex]) Guns[CurrentGunIndex]->Reload();
}

const ASGGun* Ujola6902_GunsComponent::GetGunRef() const
{
	return Guns[CurrentGunIndex];
}

void Ujola6902_GunsComponent::SetCurrentGunIndex(int8 NewIndex)
{
	CurrentGunIndex = NewIndex;
}

int8 Ujola6902_GunsComponent::GetCurrentGunIndex()
{
	return CurrentGunIndex;
}

const TArray<ASGGun*>& Ujola6902_GunsComponent::GetGuns() const
{
	return Guns;
}

// Private
void Ujola6902_GunsComponent::CreateGunsHUD()
{
	if (GunsHUDWidgetClass)
	{
		GunsHUD = CreateWidget<USGWeaponsHUD>(GetWorld(), GunsHUDWidgetClass);
		if (GunsHUD)
		{
			GunsHUD->AddToViewport();
		}
	}
}

void Ujola6902_GunsComponent::UpdateGunsHUD()
{
	if (GunsHUD && Guns[CurrentGunIndex])
	{
		GunsHUD->UpdWeaponName(Guns[CurrentGunIndex]->GetWeaponDisplayName());
		GunsHUD->UpdAmmoClip(Guns[CurrentGunIndex]->GetAmmoClip());
		GunsHUD->UpdAmmoStock(Guns[CurrentGunIndex]->GetAmmoStock());
	}
}

void Ujola6902_GunsComponent::SetUpGuns()
{
	Guns.SetNum(GunClasses.Num());
	for (int32 i = 0; i < GunClasses.Num(); ++i)
	{
		Guns[i] = GetWorld()->SpawnActor<ASGGun>(GunClasses[i]);
		if (Guns[i]) Guns[i]->SetOwner(Owner);
		if (WeaponMesh) Guns[i]->AttachToComponent(WeaponMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
}