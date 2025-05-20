// Joel Larsson Wendt | jola6902

#include "jola6902_GunsComponent.h"
#include "InputActionValue.h"
#include "SGWeaponsHUD.h"
#include "Blueprint/UserWidget.h"
#include "Gear/Weapons/SGGun.h"

// Public
Ujola6902_GunsComponent::Ujola6902_GunsComponent()
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

	Owner = GetOwner();
	CreateGunsHUD();
	SetUpGuns();
}

void Ujola6902_GunsComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	for (ASGGun* Gun : Guns)
	{
		if (Gun && IsValid(Gun))
		{
			Gun->Destroy();
		}
	}
	Guns.Empty();
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

void Ujola6902_GunsComponent::SwapWithMouseWheel(const FInputActionValue& Value)
{
	float ScrollValue = Value.Get<float>();
	
	if (FMath::IsNearlyZero(ScrollValue)) return;

	if (ScrollValue > 0.f)
	{
		CurrentGunIndex = (CurrentGunIndex + 1) % Guns.Num();
	}
	else if (ScrollValue < 0.f)
	{
		CurrentGunIndex = (CurrentGunIndex - 1 + Guns.Num()) % Guns.Num();
	}
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
	USceneComponent* AttachmentComponent = Cast<USceneComponent>(GunsAttachment.GetComponent(Owner));
	Guns.SetNum(GunClasses.Num());
	for (int32 i = 0; i < GunClasses.Num(); ++i)
	{
		Guns[i] = GetWorld()->SpawnActor<ASGGun>(GunClasses[i]);
		if (Guns[i]) Guns[i]->SetOwner(Owner);
		if (AttachmentComponent) Guns[i]->AttachToComponent(AttachmentComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
}