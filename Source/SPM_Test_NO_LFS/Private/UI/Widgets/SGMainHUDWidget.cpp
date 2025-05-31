

#include "UI/Widgets/SGMainHUDWidget.h"

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

	GetGameInstance<USGGameInstance>()->GetObjectiveTooltipWidget()->SetVisibility(ESlateVisibility::Collapsed);
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
	
	GetGameInstance<USGGameInstance>()->GetObjectiveTooltipWidget()->SetVisibility(ESlateVisibility::Visible);
}

void USGMainHUDWidget::StartDifficultyBar()
{
	HasFirstQuestStarted = true;
	if (DifficultyWidget.IsValid())
	{
		DifficultyWidget->SetVisibility(ESlateVisibility::Visible);
		DifficultyWidget->PauseDifficultyBar(false);
	}
}
