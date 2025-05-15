// Fill out your copyright notice in the Description page of Project Settings.


#include "Objectives/SGObjectiveToolTipWidget.h"
#include "Objectives/SGHorizontalBoxObjective.h"
#include "Animation/WidgetAnimation.h"
#include "Animation/WidgetAnimationEvents.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/ScaleBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"

#include "Kismet/GameplayStatics.h"
#include "Objectives/SGDifficultyBarWidget.h"
#include "Objectives/SGObjectiveBase.h"

void USGObjectiveToolTipWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (TextBlockVisitTerminal)
	{
		TextBlockVisitTerminal->SetVisibility(ESlateVisibility::Hidden);
	}
	if (DifficultyBarWidgetClass)
	{
		DifficultyBarWidget = Cast<USGDifficultyBarWidget>(CreateWidget<USGDifficultyBarWidget>(this, DifficultyBarWidgetClass));
		if (DifficultyBarWidget)
		{
			DifficultyBarWidget->AddToViewport();
			DifficultyBarWidget->SetVisibility(ESlateVisibility::Visible);
		}
			
	}
}

void USGObjectiveToolTipWidget::AddProgressTextElement(FText KeyText, FText ValueText)
{
	const float DPI = UWidgetLayoutLibrary::GetViewportScale(this);
	FMargin VerticalPadding = FMargin(80,10,80,0);
	USGHorizontalBoxObjective* NewHorizontalBox = CreateWidget<USGHorizontalBoxObjective>(this, HorizontalBoxObjectiveClass);
	NewHorizontalBox->SetKey(KeyText);
	NewHorizontalBox->SetValue(ValueText);
	NewHorizontalBox->SetValue(ValueText);
	NewHorizontalBox->HideFail();
	NewHorizontalBox->HideSucceed();
	
	// Returnerar Sloten så vi kan sätta allignment och padding på den.
	UVerticalBoxSlot* VerticalBoxSlot = VerticalBoxMission->AddChildToVerticalBox(NewHorizontalBox);
	VerticalBoxSlot->SetPadding(VerticalPadding);
	VerticalBoxSlot->SetHorizontalAlignment(HAlign_Fill);
	VerticalBoxSlot->SetVerticalAlignment(VAlign_Fill);
	NewHorizontalBox->PlayAnimationKeyStartObjective();
	HorizontalObjectiveList.Add(NewHorizontalBox);
	CurrentHorizontalBoxObjectiveElement = NewHorizontalBox;
	
}

USGHorizontalBoxObjective* USGObjectiveToolTipWidget::CreateProgressTextElement(FText KeyText, FText ValueText)
{
	const float DPI = UWidgetLayoutLibrary::GetViewportScale(this);
	FMargin VerticalPadding = FMargin(80,10,80,0);
	USGHorizontalBoxObjective* NewHorizontalBox = CreateWidget<USGHorizontalBoxObjective>(this, HorizontalBoxObjectiveClass);
	NewHorizontalBox->SetKey(KeyText);
	NewHorizontalBox->SetValue(ValueText);
	NewHorizontalBox->HideFail();
	NewHorizontalBox->HideSucceed();
	
	// Returnerar Sloten så vi kan sätta allignment och padding på den.
	UVerticalBoxSlot* VerticalBoxSlot = VerticalBoxMission->AddChildToVerticalBox(NewHorizontalBox);
	VerticalBoxSlot->SetPadding(VerticalPadding);
	VerticalBoxSlot->SetHorizontalAlignment(HAlign_Fill);
	VerticalBoxSlot->SetVerticalAlignment(VAlign_Fill);
	NewHorizontalBox->PlayAnimationKeyStartObjective();
	HorizontalObjectiveList.Add(NewHorizontalBox);
	CurrentHorizontalBoxObjectiveElement = NewHorizontalBox;
	
	return NewHorizontalBox;
}

USGHorizontalBoxObjective* USGObjectiveToolTipWidget::GetHorizontalBoxAtIndex(int32 index)
{
	if (HorizontalObjectiveList.IsValidIndex(index))
	{
		return HorizontalObjectiveList[index];	
	}

	return nullptr;
}

void USGObjectiveToolTipWidget::Display(FText NewToolTip)
{
	FString Str = "HEJ";
	SetVisibility(ESlateVisibility::HitTestInvisible);
	SetRenderOpacity(1.0f);
	ScaleBoxToolTip->SetVisibility(ESlateVisibility::HitTestInvisible);
	PlayAnimation(AnimationToolTipOutOfWindow, 0, 1);
	//ToolTip->SetText(NewToolTip);
	// = false;
	GetWorld()->GetTimerManager().SetTimer(CharByCharTimer, FTimerDelegate::CreateLambda([this, NewToolTip]()
	{
		DisplayCharByChar(NewToolTip.ToString());	
	}), 0.05f, true);
}


void USGObjectiveToolTipWidget::UpdateDifficultyBar(float InDeltaTime)
{
	DifficultyBarOffsetLeft += 10*InDeltaTime;
	DifficultyBarWidget->MoveOverlaysLeft(DifficultyBarOffsetLeft);
	int index = 0;
	//UE_LOG(LogTemp, Warning, TEXT("Trigger: %f"), DifficultyBarWidget->GetTriggerAbsolutePositionX());
	
	for (UOverlay* overlay : DifficultyBarWidget->GetOverlays())
	{
		if (overlay->GetCachedGeometry().GetAbsolutePosition().X < DifficultyBarWidget->GetTriggerAbsolutePositionX() &&
			overlay->GetCachedGeometry().GetAbsolutePosition().X > 0.0f)
		{
			// TODO (Calle): Vill Broadcasta när nästa svårighetsgrad nås!
			//UE_LOG(LogTemp, Warning, TEXT("%d: is past the Trigger"), index);
		}
		//UE_LOG(LogTemp, Warning, TEXT("%d: %f"), index, overlay->GetCachedGeometry().GetAbsolutePosition().X);
		index++;
	}
}
void USGObjectiveToolTipWidget::SetToolTipText(FText NewToolTip)
{
	ToolTip->SetText(NewToolTip);
}

void USGObjectiveToolTipWidget::ShowVisitTerminal()
{
	TextBlockVisitTerminal->SetVisibility(ESlateVisibility::HitTestInvisible);
	PlayAnimation(AnimationVisitTerminal, 0, 0);
}

void USGObjectiveToolTipWidget::HideVisitTerminal()
{
	TextBlockVisitTerminal->SetVisibility(ESlateVisibility::Hidden);
	StopAnimation(AnimationVisitTerminal);
}

void USGObjectiveToolTipWidget::ShowMissionVerticalBox()
{
	VerticalBoxMission->SetVisibility(ESlateVisibility::HitTestInvisible);
	
}

void USGObjectiveToolTipWidget::HideMissionVerticalBox()
{
	VerticalBoxMission->SetVisibility(ESlateVisibility::Hidden);
}

void USGObjectiveToolTipWidget::ShowToolTipScaleBox()
{
	ScaleBoxToolTip->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void USGObjectiveToolTipWidget::HideToolTipScaleBox()
{
	ScaleBoxToolTip->SetVisibility(ESlateVisibility::Hidden);
}

void USGObjectiveToolTipWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	UpdateDifficultyBar(InDeltaTime);
}
/*
void USGObjectiveToolTipWidget::Hide()
{
	
	bHasFadedOut = true;
	bShouldRender = false;
	bIsHidden = true;
	
}
*/
void USGObjectiveToolTipWidget::DisplayCharByChar(const FString& StringToolTip)
{
	if (++CharIndex <= StringToolTip.Len())
	{
		FString StrToDisplay = StringToolTip.Mid(0, CharIndex);
		ToolTip->SetText(FText::FromString(StrToDisplay));
		UGameplayStatics::PlaySound2D(GetWorld(), TextClickSound);
		UE_LOG(LogTemp, Error, TEXT("%s"), *StrToDisplay);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(CharByCharTimer);
		CharIndex = 0;
	}
}


void USGObjectiveToolTipWidget::OnEndMoveToolTipAnimation()
{
	StopAnimation(AnimationToolTipOutOfWindow);
}

void USGObjectiveToolTipWidget::OnEndHideToolTipAnimation()
{
	
	ScaleBoxToolTip->SetRenderTranslation(FVector2D(0.0f, 0.0f));
	ScaleBoxToolTip->SetVisibility(ESlateVisibility::Hidden);
}


