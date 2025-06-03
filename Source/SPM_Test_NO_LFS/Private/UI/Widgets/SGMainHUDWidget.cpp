

#include "UI/Widgets/SGMainHUDWidget.h"

#include "SPM_Test_NO_LFS.h"
#include "Core/SGGameInstance.h"
#include "UI/Widgets/SGHUDGrapple.h"
#include "UI/Widgets/SGWeaponsHUD.h"
#include "UI/Widgets/SGDifficultyBarWidget.h"
#include "Components/Widget.h"

void USGMainHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HasFirstQuestStarted = false;

	PauseAndHide();
	DifficultyWidget->PauseDifficultyBar(true);
	DifficultyWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void USGMainHUDWidget::PauseAndHide()
{
	if (WeaponsWidget.IsValid())
	{
		WeaponsWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (GrappleHookWidget.IsValid())
	{
		GrappleHookWidget->SetVisibility(ESlateVisibility::Collapsed);
	}	
	if (DifficultyWidget.IsValid() && HasFirstQuestStarted)
	{
		DifficultyWidget->PauseDifficultyBar(true);
		DifficultyWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	USGGameInstance* GameInstance = GetGameInstance<USGGameInstance>();
	if (!GameInstance || !GameInstance->GetObjectiveTooltipWidget())
	{
		EMMA_LOG(Warning, TEXT("USGMainHUDWidget::PauseAndHide: ObjectiveTooltipWidget is not valid!"));
		return;
	}

	GameInstance->GetObjectiveTooltipWidget()->SetVisibility(ESlateVisibility::Collapsed);
}

void USGMainHUDWidget::PlayAndShow()
{
	if (WeaponsWidget.IsValid())
	{
		WeaponsWidget->SetVisibility(ESlateVisibility::Visible);
	}
	if (GrappleHookWidget.IsValid())
	{
		GrappleHookWidget->SetVisibility(ESlateVisibility::Visible);
	}
	
	if (DifficultyWidget.IsValid() && HasFirstQuestStarted)
	{
		DifficultyWidget->PauseDifficultyBar(false);
		DifficultyWidget->SetVisibility(ESlateVisibility::Visible);
	}

	USGGameInstance* GameInstance = GetGameInstance<USGGameInstance>();
	if (!GameInstance || !GameInstance->GetObjectiveTooltipWidget())
	{
		EMMA_LOG(Warning, TEXT("USGMainHUDWidget::PlayAndShow: ObjectiveTooltipWidget is not valid!"));
		return;
	}

	if (GameInstance->GetObjectiveTooltipWidget()->GetVisibility() == ESlateVisibility::Collapsed)
	{
		GameInstance->GetObjectiveTooltipWidget()->SetVisibility(ESlateVisibility::Visible);
	}
}

void USGMainHUDWidget::StartDifficultyBar()
{
	if (HasFirstQuestStarted)
	{
		return; // Avoid starting the difficulty bar multiple times
	}
	HasFirstQuestStarted = true;
	if (DifficultyWidget.IsValid())
	{
		DifficultyWidget->SetVisibility(ESlateVisibility::Visible);
		DifficultyWidget->PauseDifficultyBar(false);
	}
}
