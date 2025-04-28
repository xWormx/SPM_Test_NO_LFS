// Fill out your copyright notice in the Description page of Project Settings.


#include "SGObjectiveToolTipWidget.h"

#include "Animation/WidgetAnimation.h"
#include "Animation/WidgetAnimationEvents.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/ScaleBox.h"
#include "Components/TextBlock.h"

void USGObjectiveToolTipWidget::NativeConstruct()
{
	Super::NativeConstruct();
	EndTimerAnimation.BindDynamic(this, &USGObjectiveToolTipWidget::SetScaleBoxTransformAfterAnimation);
	if (ShrinkAndMoveTimer)
	{
		BindToAnimationFinished(ShrinkAndMoveTimer, EndTimerAnimation);	
	}
	if (ScaleBoxTimer)
	{
		ScaleBoxTimer->SetVisibility(ESlateVisibility::Hidden);
	}
}

void USGObjectiveToolTipWidget::Display(FText NewToolTip)
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
	SetRenderOpacity(1.0f);
	CurrentOpacity = 1.0f;
	ToolTip->SetText(NewToolTip);
	bShouldRender = true;
	bHasFadedOut = false;
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
		PlayAnimation(ShrinkAndMoveTimer);
	
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
	if (!bHasFadedOut && bShouldRender)
	{
		if (CurrentOpacity > 0.0)
		{
			ToolTipBackground->SetRenderOpacity(CurrentOpacity);
			ToolTip->SetRenderOpacity(CurrentOpacity);
			CurrentOpacity -= (FadeFactor * InDeltaTime);
		}
		else
		{
			Hide();
		}
	}
}

void USGObjectiveToolTipWidget::SetToolTipText(FText NewToolTip)
{
	ToolTip->SetText(NewToolTip);
}

void USGObjectiveToolTipWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	Render(InDeltaTime);
}

void USGObjectiveToolTipWidget::Hide()
{
	//SetVisibility(ESlateVisibility::Hidden);
	ToolTipBackground->SetRenderOpacity(0.0f);
	ToolTip->SetRenderOpacity(0.0f);
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


