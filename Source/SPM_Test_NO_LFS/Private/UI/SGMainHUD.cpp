#include "UI/SGMainHUD.h"

#include "jola6902_GunsComponent.h"
#include "SPM_Test_NO_LFS.h"
#include "Components/Counters/SGCounterComponentAmmo.h"
#include "Core/SGGameInstance.h"
#include "Core/SGObjectiveHandlerSubSystem.h"
#include "UI/Widgets/SGHUDGrapple.h"
#include "Gear/Grapple/SGGrapplingHook.h"
#include "UI/Widgets/SGWeaponsHUD.h"
#include "Gear/Weapons/SGGun.h"
#include "Kismet/GameplayStatics.h"
#include "Objectives/SGTerminal.h"
#include "UI/Widgets/SGDifficultyBarWidget.h"

static bool HasFirstQuestStarted = false;

void ASGMainHUD::BeginPlay()
{
	Super::BeginPlay();

	HasFirstQuestStarted = false;
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
		DifficultyWidget->PauseDifficultyBar(true);
		DifficultyWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	GetWorld()->GetSubsystem<USGObjectiveHandlerSubSystem>()->OnObjectiveStarted.AddDynamic(this, &ASGMainHUD::WaitForObjectiveToolTipAnimation);
	GetWorld()->GetSubsystem<USGObjectiveHandlerSubSystem>()->OnObjectiveStarted.AddDynamic(this, &ASGMainHUD::StartDifficultyBar);

	Cast<USGGameInstance>(GetWorld()->GetGameInstance())->GetTerminalWidget()->OnVisibilityChanged.AddDynamic(this, &ASGMainHUD::OnTerminalVisibilityChanged);
}

void ASGMainHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	USGObjectiveHandlerSubSystem* ObjectiveHandler = GetWorld()->GetSubsystem<USGObjectiveHandlerSubSystem>();
	ObjectiveHandler->OnObjectiveStarted.RemoveDynamic(this, &ASGMainHUD::PlayAndShow);

	Super::EndPlay(EndPlayReason);
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
//	GunsComponent->OnReload.AddDynamic(WeaponsWidgetPtr, &USGWeaponsHUD::ReloadWeapon);

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

void ASGMainHUD::PauseAndHide()
{
	if (WeaponsWidget.IsValid())
	{
		WeaponsWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	if (GrappleCrossHairWidget.IsValid())
	{
		GrappleCrossHairWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	if (HUDWidget.IsValid())
	{
		HUDWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	if (DifficultyWidget.IsValid() && HasFirstQuestStarted)
	{
		DifficultyWidget->PauseDifficultyBar(true);
		DifficultyWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	GetGameInstance<USGGameInstance>()->GetObjectiveTooltipWidget()->SetVisibility(ESlateVisibility::Hidden);

}

void ASGMainHUD::PlayAndShow()
{
	if (WeaponsWidget.IsValid())
	{
		WeaponsWidget->SetVisibility(ESlateVisibility::Visible);
	}
	if (GrappleCrossHairWidget.IsValid())
	{
		GrappleCrossHairWidget->SetVisibility(ESlateVisibility::Visible);
	}
	if (HUDWidget.IsValid())
	{
		HUDWidget->SetVisibility(ESlateVisibility::Visible);
	}
	if (DifficultyWidget.IsValid() && HasFirstQuestStarted)
	{
		DifficultyWidget->PauseDifficultyBar(false);
		DifficultyWidget->SetVisibility(ESlateVisibility::Visible);
	}
    GetGameInstance<USGGameInstance>()->GetObjectiveTooltipWidget()->SetVisibility(ESlateVisibility::Visible);
}

void ASGMainHUD::OnTerminalVisibilityChanged(ESlateVisibility NewVisibility)
{
	if (NewVisibility == ESlateVisibility::Visible)
	{
		PauseAndHide();
	}
	else if (NewVisibility == ESlateVisibility::Hidden || NewVisibility == ESlateVisibility::Collapsed)
	{
		PlayAndShow();
	}
}

void ASGMainHUD::StartDifficultyBar()
{
	HasFirstQuestStarted = true;
	GetWorld()->GetSubsystem<USGObjectiveHandlerSubSystem>()->OnObjectiveStarted.RemoveDynamic(this, &ASGMainHUD::StartDifficultyBar);
}

void ASGMainHUD::WaitForObjectiveToolTipAnimation()
{
	//🤡
	if (WeaponsWidget.IsValid())
	{
		WeaponsWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	if (HUDWidget.IsValid())
	{
		HUDWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	if (DifficultyWidget.IsValid() && !HasFirstQuestStarted)
	{
		DifficultyWidget->PauseDifficultyBar(true);
		DifficultyWidget->SetVisibility(ESlateVisibility::Visible);
	}
	 GetGameInstance<USGGameInstance>()->GetObjectiveTooltipWidget()->SetVisibility(ESlateVisibility::Visible);
	//🤡
	FTimerHandle ObjectiveToolTipAnimationTimer;
	GetWorld()->GetTimerManager().SetTimer(ObjectiveToolTipAnimationTimer, this, &ASGMainHUD::PlayAndShow, 2.f, false);
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
