#include "UI/Widgets/SGDifficultyBarWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Core/SGGameInstance.h"

#include "Kismet/GameplayStatics.h"

void USGDifficultyBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
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

void USGDifficultyBarWidget::KeepIncreasingDifficultyAtLastLevel()
{
	ChangeDifficulty(CurrentDifficultyLevel);
	CurrentDifficultyLevel++;
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
			// Add another overlay box here !!!!
			// Om Svårighetsgraden är på sista steget, skapa och lägg till en overlay i Horizontalboxen
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
		if (!bCrazySlazyReached)
			UpdateDifficultyBar(InDeltaTime);	
	}
	else
	{
		if (!bCrazySlazyReached)
		{
			GetWorld()->GetTimerManager().SetTimer(
						DifficultyIncreaseTimerHandle,
						this,
					&USGDifficultyBarWidget::KeepIncreasingDifficultyAtLastLevel,
					CrazySlazyIntervallSpeed,     
					true       
					);
			OnCrazySlazy.Broadcast();
			bCrazySlazyReached = true;
		}
		
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

