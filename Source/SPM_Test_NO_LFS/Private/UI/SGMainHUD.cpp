#include "UI/SGMainHUD.h"

#include "jola6902_GunsComponent.h"
#include "SPM_Test_NO_LFS.h"
#include "Components/Counters/SGCounterComponentAmmo.h"
#include "Core/SGGameInstance.h"
#include "UI/Widgets/SGHUDGrapple.h"
#include "Gear/Grapple/SGGrapplingHook.h"
#include "UI/Widgets/SGWeaponsHUD.h"
#include "Gear/Weapons/SGGun.h"

void ASGMainHUD::BeginPlay()
{
	Super::BeginPlay();

	//TerminalWidget = CreateAndAddToViewPort<USGTerminalWidget>(TerminalClass);
	//ObjectiveWidget = CreateAndAddToViewPort<USGObjectiveToolTipWidget>(ObjectiveClass);

	GrappleCrossHairWidget = CreateAndAddToViewPort<USGHUDGrapple>(GrappleCrossHairClass);
	WeaponsWidget = CreateAndAddToViewPort<USGWeaponsHUD>(WeaponsClass);
	HUDWidget = CreateAndAddToViewPort<UUserWidget>(HUDClass);
	/*if (USGGameInstance* GameInstance = Cast<USGGameInstance>(GetGameInstance()))
	{
		TerminalWidget->SetVisibility(ESlateVisibility::Hidden);
		GameInstance->SetTerminalWidget(TerminalWidget.Get());
		EMMA_LOG(Log, TEXT("❤️TerminalWidget: %s"), *GameInstance->GetTerminalWidget()->GetName());

		ObjectiveWidget->SetVisibility(ESlateVisibility::Collapsed);
		GameInstance->SetObjectiveTooltipWidget(ObjectiveWidget.Get());
		EMMA_LOG(Log, TEXT("❤️ObjectiveToolTipWidget: %s"), *GameInstance->GetObjectiveTooltipWidget()->GetName());
	}*/
}

void ASGMainHUD::BindToGrappleEvents(ASGGrapplingHook* GrapplingHook)
{
	if (!GrapplingHook)
	{
		return;
	}

	GrapplingHook->OnFireGrapple.AddDynamic(this, &ASGMainHUD::OnFireGrapple);
	GrapplingHook->OnCanGrapple.AddDynamic(this, &ASGMainHUD::OnCanGrapple);
	GrapplingHook->OnBeginCooldown.AddDynamic(this, &ASGMainHUD::OnBeginGrappleCooldown);
}

void ASGMainHUD::BindWeaponEvents(Ujola6902_GunsComponent* GunsComponent)
{
	if (!GunsComponent)
	{
		return;
	}
	GunsComponent->OnSwitchedGun.AddDynamic(this, &ASGMainHUD::OnSwitchedGun);
	GunsComponent->OnFireGun.AddDynamic(this, &ASGMainHUD::OnFireGun);
	GunsComponent->OnReload.AddDynamic(this, &ASGMainHUD::OnReload);
	GunsComponent->OnCanFireGun.AddDynamic(this, &ASGMainHUD::OnCanFireGun);

	WeaponsWidget->SetAvailableWeapons(GunsComponent->GetGuns());
	OnSwitchedGun(0, GunsComponent->GetGuns()[0]);
}

void ASGMainHUD::BindToAmmoEvents(USGCounterComponentAmmo* AmmoComponent)
{
	if (!AmmoComponent)
	{
		return;
	}
	AmmoComponent->OnPickedUpAmmo.AddDynamic(this, &ASGMainHUD::OnPickedUpAmmo);
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

void ASGMainHUD::OnCanGrapple(bool bCanGrapple)
{
	if (!bCanGrapple)
	{
		GrappleCrossHairWidget->PlayValidTargetAnimation();
	}
}

//----WEAPONS

void ASGMainHUD::OnSwitchedGun(const int32 GunIndex, ASGGun* Gun)
{
	WeaponsWidget->OnWeaponChanged(GunIndex);
}

void ASGMainHUD::OnFireGun(const int32 GunIndex, ASGGun* Gun)
{
	EMMA_LOG(Log, TEXT("❤️Firing gun %d"), GunIndex);
	WeaponsWidget->UpdateWeapon(GunIndex, Gun);
}

void ASGMainHUD::OnReload(const int32 GunIndex, ASGGun* Gun)
{
	EMMA_LOG(Log, TEXT("❤️Reloading gun %d"), GunIndex);
	WeaponsWidget->UpdateWeapon(GunIndex, Gun);
}

void ASGMainHUD::OnCanFireGun(bool bCanFire, int32 GunIndex, ASGGun* Gun)
{
	EMMA_LOG(Log, TEXT("❤️Can fire gun %d: %s"), GunIndex, bCanFire ? TEXT("true") : TEXT("false"));
}

void ASGMainHUD::OnPickedUpAmmo(const int32 AmmoAmount, ASGGun* Gun)
{
	WeaponsWidget->UpdateAmmo(AmmoAmount, Gun);
}

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
