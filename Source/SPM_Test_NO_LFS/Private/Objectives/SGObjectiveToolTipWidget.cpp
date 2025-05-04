// Fill out your copyright notice in the Description page of Project Settings.


#include "Objectives/SGObjectiveToolTipWidget.h"

#include "Animation/WidgetAnimation.h"
#include "Animation/WidgetAnimationEvents.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/ScaleBox.h"
#include "Components/TextBlock.h"
#include "Objectives/SGObjectiveBase.h"

void USGObjectiveToolTipWidget::NativeConstruct()
{
	Super::NativeConstruct();
	EndTimerAnimation.BindDynamic(this, &USGObjectiveToolTipWidget::SetScaleBoxTransformAfterAnimation);
	if (ShrinkAndMoveTimer)
	{
		BindToAnimationFinished(ShrinkAndMoveTimer, EndTimerAnimation);	
	}
	EndMoveToolTipToProgressWindowAnimation.BindDynamic(this, &USGObjectiveToolTipWidget::OnEndMoveToolTipAnimation);
	if (MoveToolTipToProgressWindow)
	{
		BindToAnimationFinished(MoveToolTipToProgressWindow, EndMoveToolTipToProgressWindowAnimation);
	}
	EndHideToolTipAnimation.BindDynamic(this, &USGObjectiveToolTipWidget::OnEndHideToolTipAnimation);
	if (AnimationHideToolTip)
	{
		BindToAnimationFinished(AnimationHideToolTip, EndHideToolTipAnimation);
	}
	if (ScaleBoxTimer)
	{
		ScaleBoxTimer->SetVisibility(ESlateVisibility::Hidden);
	}
	if (ScaleBoxMission)
	{
		ScaleBoxMission->SetVisibility(ESlateVisibility::Hidden);
	}
	
}

void USGObjectiveToolTipWidget::Display(FText NewToolTip)
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
	SetRenderOpacity(1.0f);
	ScaleBoxToolTip->SetVisibility(ESlateVisibility::HitTestInvisible);
	PlayAnimation(MoveToolTipToProgressWindow, 0, 1);
	ToolTip->SetText(NewToolTip);
	bIsHidden = false;
}

void USGObjectiveToolTipWidget::DisplayTimer(FText NewTimerText)
{
	ScaleBoxTimer->SetVisibility(ESlateVisibility::HitTestInvisible);
	TextTimer->SetText(NewTimerText);
	UE_LOG(LogTemp, Warning, TEXT("ScaleBox opacity: %f, RenderOpacity: %f, Visibility: %d"), ScaleBoxTimer->GetRenderOpacity(), GetRenderOpacity(), GetIsHidden());
	if (UCanvasPanelSlot* ScaleBoxSlot = Cast<UCanvasPanelSlot>(ScaleBoxTimer->Slot))
	{
		if (!bTimerAnimationFinished)
		{
			ScaleBoxTimerFinalPosition = ScaleBoxSlot->GetPosition();
			ScaleBoxTimerFinalSize = ScaleBoxSlot->GetSize();
		}
	}
	if (!bTimerAnimationFinished)
		PlayAnimation(ShrinkAndMoveTimer, 0 ,1);
	
	if (IsAnimationPlaying(ShrinkAndMoveTimer))
	{
		UE_LOG(LogTemp, Warning, TEXT("ShrinkAndMoveTimer is playing"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ShrinkAndMoveTimer is NOOOT playing"));
	}

}

void USGObjectiveToolTipWidget::Render(float InDeltaTime)
{
}

void USGObjectiveToolTipWidget::SetToolTipText(FText NewToolTip)
{
	ToolTip->SetText(NewToolTip);
}

void USGObjectiveToolTipWidget::SetProgressWindowText(ASGObjectiveBase* Objective)
{
	if (!Objective->GetProgressText().SubText.IsEmpty())
	{
		if (!ProgressTextMap.Contains(Objective->GetObjectiveID()))
		{
			ProgressTextMap.Add(Objective->GetObjectiveID(), Objective->GetProgressText());
		}
		for (int i = 0; i < Objective->GetCurrentProgressStep(); i++)
		{
			if (i >= Objective->GetProgressText().SubText.Num())
			{
				UE_LOG(LogTemp, Error, TEXT("ProgressStep was larger than number of ProgressTexts"));
				break;
			}
			if (i == 0)
			{
				TextBlockMissionProgress->SetText(FText::FromString(TEXT("- ") + Objective->GetProgressText().SubText[i]));
				continue;
				
			}
			FString CurrentProgressText = TextBlockMissionProgress->GetText().ToString();
			TextBlockMissionProgress->SetText(FText::FromString(CurrentProgressText + ("\n\t- ") + Objective->GetProgressText().SubText[i]));
		}
		
	}
}

void USGObjectiveToolTipWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	Render(InDeltaTime);
}

void USGObjectiveToolTipWidget::Hide()
{
	
	CurrentOpacity = 1.0f;
	bHasFadedOut = true;
	bShouldRender = false;
	bIsHidden = true;
}

void USGObjectiveToolTipWidget::SetScaleBoxTransformAfterAnimation()
{
	UE_LOG(LogTemp, Warning, TEXT("End of ToolTipWidget Animation!"));
	bTimerAnimationFinished = true;
	// Hämtar position och storlek vid slutet av animationen och sätter den till den nya riktiga pos/size.
	if (UCanvasPanelSlot* ScaleBoxSlot = Cast<UCanvasPanelSlot>(ScaleBoxTimer->Slot))
	{
		ScaleBoxSlot->SetPosition(ScaleBoxTimerFinalPosition);
		ScaleBoxSlot->SetSize(ScaleBoxTimerFinalSize);
	}
	StopAnimation(ShrinkAndMoveTimer);
}

void USGObjectiveToolTipWidget::OnEndMoveToolTipAnimation()
{
	StopAnimation(MoveToolTipToProgressWindow);
	PlayAnimation(AnimationHideToolTip, 0, 1);
}

void USGObjectiveToolTipWidget::OnEndHideToolTipAnimation()
{
	StopAnimation(AnimationHideToolTip);
	ScaleBoxToolTip->SetRenderTranslation(FVector2D(0.0f, 0.0f));
	ScaleBoxToolTip->SetVisibility(ESlateVisibility::Hidden);
	ScaleBoxMission->SetVisibility(ESlateVisibility::HitTestInvisible);
}


