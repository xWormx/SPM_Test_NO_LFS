#include "UI/Widgets/SGDifficultyBarWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Core/SGGameInstance.h"

#include "Kismet/GameplayStatics.h"

void USGDifficultyBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	/*if (USGGameInstance* GameInstance = Cast<USGGameInstance>(GetGameInstance()))
	{
		GameInstance->OnDifficultyIncreased.AddDynamic(this, &USGDifficultyBarWidget::AnimationTriggerAlert);

		/*ObjectiveToolTipWidget = GameInstance->GetObjectiveTooltipWidget();
		if (ObjectiveToolTipWidget)
		{
			ObjectiveToolTipWidget->OnDifficultyChanged.AddDynamic(this, &USGDifficultyBarWidget::AnimationTriggerAlert);
			ObjectiveToolTipWidget->OnDifficultyChanged.AddDynamic(GameInstance, &USGGameInstance::IncreaseDifficultyLevel);	
		}#1#
	}*/
/*
	OverlayWarningMessage->SetVisibility(ESlateVisibility::Hidden);
	Overlays.Add(OverlayDifficulty);
	Overlays.Add(OverlayDifficulty_1);
	Overlays.Add(OverlayDifficulty_2);
	Overlays.Add(OverlayDifficulty_3);
*/
	
	TArray<UWidget*> AllWidgets;
	WidgetTree->GetAllWidgets(AllWidgets);

	for (UWidget* Widget : AllWidgets)
	{
		if (UOverlay* Overlay = Cast<UOverlay>(Widget))
		{
			if (Overlay->GetName().Contains("OverlayDifficulty"))
			{
				Overlays.Add(Overlay);
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Found %d overlays"), Overlays.Num());
}

void USGDifficultyBarWidget::PauseDifficultyBar(const bool bPauseBar)
{
	bPause = bPauseBar;
}

float USGDifficultyBarWidget::GetTriggerAbsolutePositionX() const
{
	return ImageTrigger->GetCachedGeometry().GetAbsolutePosition().X;
}

void USGDifficultyBarWidget::UpdateDifficultyBar(const float InDeltaTime)
{
	DifficultyBarOffsetLeft += DifficultBarScrollSpeed*InDeltaTime;
	const FVector2D NewPosition = FVector2D(DifficultBoxStartPosition - DifficultyBarOffsetLeft, 0.0f);
	const float TriggerAbsolutePosition = GetTriggerAbsolutePositionX();

	int DifficultyIndex = 0;
	for (UOverlay* Overlay : Overlays)
	{
		DifficultyIndex++;
		Overlay->SetRenderTranslation(NewPosition);

		if (DifficultyIndex <= CurrentDifficultyLevel)
		{
			continue;
		}

		const float OverlayPositionX = Overlay->GetCachedGeometry().GetAbsolutePosition().X;

		if (OverlayPositionX < TriggerAbsolutePosition && OverlayPositionX > 0.0f)
		{
			ChangeDifficulty(DifficultyIndex);
		}
	}
}

void USGDifficultyBarWidget::ChangeDifficulty(const int32 NextDifficultyLevel)
{
	CurrentDifficultyLevel = NextDifficultyLevel;

	const USGGameInstance* GameInstance = Cast<USGGameInstance>(GetGameInstance());
	GameInstance->OnDifficultyIncreased.Broadcast(CurrentDifficultyLevel);
	
	PlayAnimation(AnimationShowWarningMessage);
	PlayAnimation(AnimationTriggerNewDifficultLevel);

	const UWorld* World = GetWorld();
	UGameplayStatics::PlaySound2D(World, SoundWarningDifficultLevel);
	UGameplayStatics::PlaySound2D(World, SoundAlarmBell);
}

void USGDifficultyBarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);	

	if (bPause)
	{
		return;
	}
	if(LastDifficultNotReached() || LastDifficultBoxNotCenteredAtTrigger())
	{
		UpdateDifficultyBar(InDeltaTime);	
	}
}

bool USGDifficultyBarWidget::LastDifficultNotReached()
{
	const UOverlay* LastOverlay = Overlays.Last();
	const float LastOverlayAbsolutePosition = LastOverlay->GetCachedGeometry().GetAbsolutePosition().X;
	const float WhereToStop = GetTriggerAbsolutePositionX() - LastOverlay->GetCachedGeometry().GetAbsoluteSize().X / 2;

	return LastOverlayAbsolutePosition >= WhereToStop && LastOverlayAbsolutePosition > 0.f;
}

bool USGDifficultyBarWidget::LastDifficultBoxNotCenteredAtTrigger() const
{
	return CurrentDifficultyLevel != Overlays.Num();
}

/*void USGDifficultyBarWidget::MoveOverlaysLeft(const float TranslationMovement)
{
	for (UOverlay* Overlay : Overlays)
	{
		const FVector2D NewPosition = FVector2D(DifficultBoxStartPosition - TranslationMovement, 0.0f);
		Overlay->SetRenderTranslation(NewPosition);
	}
}*/
/*float USGDifficultyBarWidget::GetDifficultBarScrollSpeed() const
{
	return DifficultBarScrollSpeed;
}*/
/*
TArray<UOverlay*> USGDifficultyBarWidget::GetOverlays()
{
	return Overlays;
}
*/
/*void USGDifficultyBarWidget::AnimationTriggerAlert(int NewDifficultLevel)
{
	DifficultLevel = NewDifficultLevel;
	PlayAnimation(AnimationShowWarningMessage);
	PlayAnimation(AnimationTriggerNewDifficultLevel);
}*/

