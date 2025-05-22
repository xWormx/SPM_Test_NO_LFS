#include "UI/SGMainHUD.h"
#include "SPM_Test_NO_LFS.h"
#include "Core/SGGameInstance.h"
#include "UI/Widgets/SGHUDGrapple.h"
#include "Gear/Grapple/SGGrapplingHook.h"
#include "Objectives/SGTerminalWidget.h"
#include "Objectives/SGObjectiveToolTipWidget.h"

void ASGMainHUD::BeginPlay()
{
	Super::BeginPlay();

	//TerminalWidget = CreateAndAddToViewPort<USGTerminalWidget>(TerminalClass);
	//ObjectiveWidget = CreateAndAddToViewPort<USGObjectiveToolTipWidget>(ObjectiveClass);

	GrappleCrossHairWidget = CreateAndAddToViewPort<USGHUDGrapple>(GrappleCrossHairClass);
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
