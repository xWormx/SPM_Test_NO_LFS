// Fill out your copyright notice in the Description page of Project Settings.


#include "SGObjectiveToolTipWidget.h"

#include "Animation/WidgetAnimation.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void USGObjectiveToolTipWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void USGObjectiveToolTipWidget::Display(FText NewToolTip)
{
	SetVisibility(ESlateVisibility::Visible);
	SetRenderOpacity(1.0f);
	CurrentOpacity = 1.0f;
	ToolTip->SetText(NewToolTip);
	TextTimer->SetText(FText::FromString(""));
	TextTimer->SetIsEnabled(false);
	bShouldRender = true;
	bHasFadedOut = false;
	bIsHidden = false;
}

void USGObjectiveToolTipWidget::DisplayTimer(FText NewTimerText)
{
	TextTimer->SetText(NewTimerText);
	TextTimer->SetIsEnabled(true);
	PlayAnimation(ShrinkAndMoveTimer);
	//ShrinkAndMoveTimer->BindToAnimationFinished()
}

void USGObjectiveToolTipWidget::Render(float InDeltaTime)
{
	if (!bHasFadedOut && bShouldRender)
	{
		if (GetRenderOpacity() > 0.0)
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
	SetVisibility(ESlateVisibility::Hidden);
	CurrentOpacity = 1.0f;
	bHasFadedOut = true;
	bShouldRender = false;
	bIsHidden = true;
}


