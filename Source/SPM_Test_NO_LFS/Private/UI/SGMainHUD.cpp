#include "UI/SGMainHUD.h"

#include "jola6902_GunsComponent.h"
#include "SPM_Test_NO_LFS.h"
#include "Components/Counters/SGCounterComponentAmmo.h"
#include "UI/Widgets/SGHUDGrapple.h"
#include "Gear/Grapple/SGGrapplingHook.h"
#include "UI/Widgets/SGWeaponsHUD.h"
#include "Gear/Weapons/SGGun.h"
#include "UI/Widgets/SGDifficultyBarWidget.h"

void ASGMainHUD::BeginPlay()
{
	Super::BeginPlay();

	if (!GrappleCrossHairWidget.IsValid())
	{
		GrappleCrossHairWidget = CreateAndAddToViewPort<USGHUDGrapple>(GrappleCrossHairClass);
	}

	if (!WeaponsWidget.IsValid())
	{
		WeaponsWidget = CreateAndAddToViewPort<USGWeaponsHUD>(WeaponsClass);
	}

	if (!HUDWidget.IsValid())
	{
		HUDWidget = CreateAndAddToViewPort<UUserWidget>(HUDClass);
	}

	if (!DifficultyWidget.IsValid())
	{
		DifficultyWidget = CreateAndAddToViewPort<USGDifficultyBarWidget>(DifficultyClass);
	}
}

void ASGMainHUD::BindToGrappleEvents(ASGGrapplingHook* GrapplingHook)
{
	if (!GrapplingHook)
	{
		return;
	}

	if (!GrappleCrossHairWidget.IsValid())
	{
		GrappleCrossHairWidget = CreateAndAddToViewPort<USGHUDGrapple>(GrappleCrossHairClass);
	}

	GrapplingHook->OnFireGrapple.AddDynamic(this, &ASGMainHUD::OnFireGrapple);
	GrapplingHook->OnCanGrapple.AddDynamic(GrappleCrossHairWidget.Get(), &USGHUDGrapple::PlayValidTargetAnimation);
	GrapplingHook->OnBeginCooldown.AddDynamic(this, &ASGMainHUD::OnBeginGrappleCooldown);
}

void ASGMainHUD::BindWeaponEvents(Ujola6902_GunsComponent* GunsComponent)
{
	if (!GunsComponent)
	{
		return;
	}

	if (!WeaponsWidget.IsValid())
	{
		WeaponsWidget = CreateAndAddToViewPort<USGWeaponsHUD>(WeaponsClass);
	}

	USGWeaponsHUD* WeaponsWidgetPtr = WeaponsWidget.Get();

	GunsComponent->OnSwitchedGun.AddDynamic(WeaponsWidgetPtr, &USGWeaponsHUD::ChangeWeapon);
	GunsComponent->OnFireGun.AddDynamic(WeaponsWidgetPtr, &USGWeaponsHUD::UpdateWeapon);
	GunsComponent->OnReload.AddDynamic(WeaponsWidgetPtr, &USGWeaponsHUD::UpdateWeapon);

	WeaponsWidget->SetAvailableWeapons(GunsComponent->GetGuns());
	WeaponsWidget->ChangeWeapon(0, GunsComponent->GetGuns()[0]);
}

void ASGMainHUD::BindToAmmoEvents(USGCounterComponentAmmo* AmmoComponent)
{
	if (!AmmoComponent)
	{
		return;
	}
	if (!WeaponsWidget.IsValid() || WeaponsWidget.IsStale())
	{
		WeaponsWidget = CreateAndAddToViewPort<USGWeaponsHUD>(WeaponsClass);
	}

	USGWeaponsHUD* WeaponsWidgetPtr = WeaponsWidget.Get();
	AmmoComponent->OnPickedUpAmmo.AddDynamic(WeaponsWidgetPtr, &USGWeaponsHUD::UpdateAmmo);

}

//----GRAPPLING
void ASGMainHUD::OnBeginGrappleCooldown(FTimerHandle& GrappleTimerHandle)
{
	const float TimeRemaining = GetWorld()->GetTimerManager().GetTimerRemaining(GrappleTimerHandle);
	EMMA_LOG(Log, TEXT("❤️Grapple Cooldown: %f"), TimeRemaining);
}

void ASGMainHUD::OnFireGrapple()
{
	EMMA_LOG(Log, TEXT("❤️Grapple Fired!"));
}

//----HELPERS
template <typename T>
T* ASGMainHUD::CreateAndAddToViewPort(const TSubclassOf<T>& WidgetClass, const bool Add)
{
	if (!WidgetClass)
	{
		return nullptr;
	}

	T* Widget = CreateWidget<T>(GetWorld(), WidgetClass);

	if (!Widget)
	{
		return nullptr;
	}

	if (Add)
	{
		Widget->AddToViewport();
	}

	return Widget;
}
