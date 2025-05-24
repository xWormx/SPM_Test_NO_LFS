// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/SGDifficultyBarWidget.h"

#include "SPM_Test_NO_LFS.h"
#include "Components/Overlay.h"
#include "Core/SGGameInstance.h"

#include "Kismet/GameplayStatics.h"

void USGDifficultyBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (USGGameInstance* GameInstance = Cast<USGGameInstance>(GetGameInstance()))
	{
		//GameInstance->OnDifficultyIncreased.AddDynamic(this, &USGDifficultyBarWidget::AnimationTriggerAlert);

		/*ObjectiveToolTipWidget = GameInstance->GetObjectiveTooltipWidget();
		if (ObjectiveToolTipWidget)
		{
			ObjectiveToolTipWidget->OnDifficultyChanged.AddDynamic(this, &USGDifficultyBarWidget::AnimationTriggerAlert);
			ObjectiveToolTipWidget->OnDifficultyChanged.AddDynamic(GameInstance, &USGGameInstance::IncreaseDifficultyLevel);	
		}*/
	}

	OverlayWarningMessage->SetVisibility(ESlateVisibility::Hidden);
	Overlays.Add(OverlayDifficulty);
	Overlays.Add(OverlayDifficulty_1);
	Overlays.Add(OverlayDifficulty_2);
	Overlays.Add(OverlayDifficulty_3);

	TriggerPosition = ImageTrigger->GetCachedGeometry().GetAbsolutePosition().X;
}

void USGDifficultyBarWidget::MoveOverlaysLeft(const float TranslationMovement)
{
	for (UOverlay* Overlay : Overlays)
	{
		const FVector2D NewPosition = FVector2D(DifficultBoxStartPosition - TranslationMovement, 0.0f);
		Overlay->SetRenderTranslation(NewPosition);
	}
}

float USGDifficultyBarWidget::GetTriggerAbsolutePositionX() const
{
	return ImageTrigger->GetCachedGeometry().GetAbsolutePosition().X;
}

float USGDifficultyBarWidget::GetDifficultBarScrollSpeed() const
{
	return DifficultBarScrollSpeed;
}

TArray<UOverlay*> USGDifficultyBarWidget::GetOverlays()
{
	return Overlays;
}

void USGDifficultyBarWidget::UpdateDifficultyBar(const float InDeltaTime)
{
	DifficultyBarOffsetLeft += GetDifficultBarScrollSpeed()*InDeltaTime;
	MoveOverlaysLeft(DifficultyBarOffsetLeft);

	int Index = 0;

	const float TriggerAbsolutePosition = GetTriggerAbsolutePositionX();
	
	for (const UOverlay* Overlay : GetOverlays())
	{
		float OverlayPosX= Overlay->GetCachedGeometry().GetAbsolutePosition().X;
		if (OverlayPosX < TriggerAbsolutePosition && OverlayPosX > 0.0f)
		{
			if (Index + 1 > DifficultLevel)
			{
				ChangeDifficulty(Index + 1);
				/*DifficultLevel = index+1;
				Cast<USGGameInstance>(GetGameInstance())->OnDifficultyIncreased.Broadcast(DifficultLevel);
				UGameplayStatics::PlaySound2D(GetWorld(), SoundWarningDifficultLevel);
				UGameplayStatics::PlaySound2D(GetWorld(), SoundAlarmBell);*/		
			}
		}
		Index++;
	}
}

void USGDifficultyBarWidget::ChangeDifficulty(const int32 NextDifficultyLevel)
{
	DifficultLevel = NextDifficultyLevel;
	
	Cast<USGGameInstance>(GetGameInstance())->OnDifficultyIncreased.Broadcast(DifficultLevel);
	
	PlayAnimation(AnimationShowWarningMessage);
	PlayAnimation(AnimationTriggerNewDifficultLevel);

	const UWorld* World = GetWorld();
	UGameplayStatics::PlaySound2D(World, SoundWarningDifficultLevel);
	UGameplayStatics::PlaySound2D(World, SoundAlarmBell);
	EMMA_LOG(Error, TEXT("Difficulty Level changed to: %d"), DifficultLevel);

}

void USGDifficultyBarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);	

	if(LastDifficultNotReached() || LastDifficultBoxNotCenteredAtTrigger())
	{
		UpdateDifficultyBar(InDeltaTime);	
	}
}

bool USGDifficultyBarWidget::HasReachedMaxDifficultyPosition()
{
	static const UOverlay* LastOverlay = Overlays.Last();

	static const float PositionX = LastOverlay->GetCachedGeometry().GetAbsolutePosition().X;
	static const float Width = LastOverlay->GetCachedGeometry().GetAbsoluteSize().X;
	static const float WhereToStop = TriggerPosition - Width / 2;

	return PositionX < WhereToStop && PositionX < 0;
}
bool USGDifficultyBarWidget::LastDifficultNotReached()
{
	float LastOverlayAbsolutePosition = GetOverlays().Last()->GetCachedGeometry().GetAbsolutePosition().X;
	float WhereToStop = GetTriggerAbsolutePositionX() - GetOverlays().Last()->GetCachedGeometry().GetAbsoluteSize().X / 2;

	return (LastOverlayAbsolutePosition >= WhereToStop) && (LastOverlayAbsolutePosition >= 0);
}
bool USGDifficultyBarWidget::HasReachedMaxDifficulty() const
{
	return DifficultLevel == Overlays.Num();
}

bool USGDifficultyBarWidget::LastDifficultBoxNotCenteredAtTrigger()
{
	return DifficultLevel != GetOverlays().Num();
}

void USGDifficultyBarWidget::AnimationTriggerAlert(int NewDifficultLevel)
{
	EMMA_LOG(Error, TEXT("Difficulty Level changed to: %d"), NewDifficultLevel);
	DifficultLevel = NewDifficultLevel;
	PlayAnimation(AnimationShowWarningMessage);
	PlayAnimation(AnimationTriggerNewDifficultLevel);
}

