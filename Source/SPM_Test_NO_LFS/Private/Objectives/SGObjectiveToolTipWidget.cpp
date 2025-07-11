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
#include "Core/SGObjectiveHandlerSubSystem.h"

#include "Kismet/GameplayStatics.h"
#include "UI/Widgets/SGDifficultyBarWidget.h"
#include "Objectives/SGObjectiveBase.h"

void USGObjectiveToolTipWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (OverlayVisitTerminal)
	{
		OverlayVisitTerminal->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	if (TextBlockVisitTerminal)
	{
		TextBlockVisitTerminal->SetVisibility(ESlateVisibility::Hidden);
	}
}

USGHorizontalBoxObjective* USGObjectiveToolTipWidget::CreateProgressTextElement(int32 ObjectiveID, FText KeyText, FText ValueText)
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
	
	USGObjectiveHandlerSubSystem* ObjectiveHandler = GetWorld()->GetSubsystem<USGObjectiveHandlerSubSystem>();
	AddHorizontalBoxToMap(ObjectiveID, NewHorizontalBox);
	
	CurrentHorizontalBoxObjectiveElement = NewHorizontalBox;
	
	//	NewHorizontalBox->PlayAnimationKeyStartObjective();
	
	//Detta verkar funka då animation sker för tidigt annars, innan widgeten har intialiserats
	FTimerHandle TempHandle;
	GetWorld()->GetTimerManager().SetTimer(TempHandle, [NewHorizontalBox]()
	{
		NewHorizontalBox->PlayAnimationKeyStartObjective();
	}, 0.01f, false);
	
	return NewHorizontalBox;
}

USGHorizontalBoxObjective* USGObjectiveToolTipWidget::CreateProgressTextElementCompleted(FText KeyText, FText ValueText)
{
	const float DPI = UWidgetLayoutLibrary::GetViewportScale(this);
	FMargin VerticalPadding = FMargin(80,10,80,0);
	USGHorizontalBoxObjective* NewHorizontalBox = CreateWidget<USGHorizontalBoxObjective>(this, HorizontalBoxObjectiveClass);
	NewHorizontalBox->SetKey(KeyText);
	NewHorizontalBox->SetValue(ValueText);
	NewHorizontalBox->HideFail();
	NewHorizontalBox->ShowSucceed();
	NewHorizontalBox->SetKeyOpacity(0.5f);
	NewHorizontalBox->SetValueOpacity(0.5f);
	
	// Returnerar Sloten så vi kan sätta allignment och padding på den.
	UVerticalBoxSlot* VerticalBoxSlot = VerticalBoxMission->AddChildToVerticalBox(NewHorizontalBox);
	VerticalBoxSlot->SetPadding(VerticalPadding);
	VerticalBoxSlot->SetHorizontalAlignment(HAlign_Fill);
	VerticalBoxSlot->SetVerticalAlignment(VAlign_Fill);
	//HorizontalObjectiveList.Add(NewHorizontalBox);
	CurrentHorizontalBoxObjectiveElement = NewHorizontalBox;
	
	return NewHorizontalBox;
}

void USGObjectiveToolTipWidget::AddHorizontalBoxToMap(int32 ObjectiveID, USGHorizontalBoxObjective* NewWidget)
{
	if (HorizontalBoxObjectiveMap.Contains(ObjectiveID))
	{
		HorizontalBoxObjectiveMap[ObjectiveID].HorizontalBoxElements.Add(NewWidget);
	}
	else
	{
		FHorizontalBoxList NewList;
		NewList.HorizontalBoxElements.Add(NewWidget);
		HorizontalBoxObjectiveMap.Add(ObjectiveID, NewList);
	}
}

USGHorizontalBoxObjective* USGObjectiveToolTipWidget::GetCurrentHorizontalBoxObjective()
{
	USGObjectiveHandlerSubSystem* ObjectiveHandler = GetWorld()->GetSubsystem<USGObjectiveHandlerSubSystem>(); 
	
	ASGObjectiveBase* Objective = ObjectiveHandler->GetCurrentObjective();
	int32 SubObjectiveStep = Objective->GetCurrentProgressStep();
	int32 ObjectiveID = Objective->GetObjectiveID();
	
	if (HorizontalBoxObjectiveMap.Contains(ObjectiveID))
	{
		FHorizontalBoxList& List = HorizontalBoxObjectiveMap[ObjectiveID];
		if (List.HorizontalBoxElements.IsValidIndex(SubObjectiveStep))
		{
			USGHorizontalBoxObjective* HorizontalBoxElement = List.HorizontalBoxElements[SubObjectiveStep];
			return HorizontalBoxElement;
		}
	}
	return nullptr;
}

USGHorizontalBoxObjective* USGObjectiveToolTipWidget::GetHorizontalBoxObjective(ASGObjectiveBase* Objective, int32 BoxIndex)
{
	int32 SubObjectiveStep = Objective->GetCurrentProgressStep();
	int32 ObjectiveID = Objective->GetObjectiveID();
	
	if (HorizontalBoxObjectiveMap.Contains(ObjectiveID))
	{
		FHorizontalBoxList& List = HorizontalBoxObjectiveMap[ObjectiveID];
		if (List.HorizontalBoxElements.IsValidIndex(BoxIndex))
		{
			USGHorizontalBoxObjective* HorizontalBoxElement = List.HorizontalBoxElements[BoxIndex];
			return HorizontalBoxElement;
		}
	}
	return nullptr;
}

void USGObjectiveToolTipWidget::Display(FText NewToolTip)
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
	SetRenderOpacity(1.0f);
	ScaleBoxToolTip->SetVisibility(ESlateVisibility::HitTestInvisible);
	PlayAnimation(AnimationToolTipOutOfWindow, 0, 1);
	//ToolTip->SetText(NewToolTip);
	GetWorld()->GetTimerManager().SetTimer(CharByCharTimer, FTimerDelegate::CreateLambda([this, NewToolTip]()
	{
		DisplayCharByChar(NewToolTip.ToString());	
	}), 0.05f, true);
}

void USGObjectiveToolTipWidget::DeInitialize()
{
	CurrentHorizontalBoxObjectiveElement = nullptr;
	RemoveFromParent();
}

void USGObjectiveToolTipWidget::PauseAllOngoingAnimations()
{
	for (const TPair<int32, FHorizontalBoxList>& Pair : HorizontalBoxObjectiveMap)
	{
		const FHorizontalBoxList& BoxList = Pair.Value;

		for (USGHorizontalBoxObjective* HorizontalBoxObject : BoxList.HorizontalBoxElements)
		{
			if (HorizontalBoxObject)
			{
				HorizontalBoxObject->PauseAllOngoingAnimations();
			}
		}
	}

	if (IsAnimationPlaying(AnimationVisitTerminal))
	{
		bAnimationVisitTerminalWasPaused = true;
		AnimationVisitTerminalPauseTime = PauseAnimation(AnimationVisitTerminal);
	}
		
	if (IsAnimationPlaying(AnimationToolTipOutOfWindow))
	{
		bAnimationToolTipOutOfWindowWasPaused = true;
		AnimationToolTipOutOfWindowPauseTime = PauseAnimation(AnimationToolTipOutOfWindow);
	}
		
	
}

void USGObjectiveToolTipWidget::ResumeAllOngoingAnimations()
{
	for (const TPair<int32, FHorizontalBoxList>& Pair : HorizontalBoxObjectiveMap)
	{
		const FHorizontalBoxList& BoxList = Pair.Value;

		for (USGHorizontalBoxObjective* HorizontalBoxObject : BoxList.HorizontalBoxElements)
		{
			if (HorizontalBoxObject)
			{
				HorizontalBoxObject->PauseAllOngoingAnimations();
			}
		}
	}

	if (bAnimationToolTipOutOfWindowWasPaused)
	{
		bAnimationToolTipOutOfWindowWasPaused = false;
		PlayAnimation(AnimationToolTipOutOfWindow, AnimationToolTipOutOfWindowPauseTime);
	}
	
	if (bAnimationVisitTerminalWasPaused)
	{
		bAnimationVisitTerminalWasPaused = false;
		PlayAnimation(AnimationVisitTerminal, AnimationVisitTerminalPauseTime, 0);	
	}
}

void USGObjectiveToolTipWidget::SetToolTipText(FText NewToolTip)
{
	ToolTip->SetText(NewToolTip);
}

void USGObjectiveToolTipWidget::ShowVisitTerminal()
{
	OverlayVisitTerminal->SetVisibility(ESlateVisibility::HitTestInvisible);
	TextBlockVisitTerminal->SetVisibility(ESlateVisibility::HitTestInvisible);
	
	PlayAnimation(AnimationVisitTerminal, 0, 0);
}

void USGObjectiveToolTipWidget::HideVisitTerminal()
{
	OverlayVisitTerminal->SetVisibility(ESlateVisibility::Hidden);
	TextBlockVisitTerminal->SetVisibility(ESlateVisibility::Hidden);
	StopAnimation(AnimationVisitTerminal);
	
}

void USGObjectiveToolTipWidget::SetMissionAlertText(FString NewText)
{
	TextBlockVisitTerminal->SetText(FText::FromString(NewText));
}

void USGObjectiveToolTipWidget::PlayEscapeWithPodAnimation(FString TextToDisplay)
{
	OverlayVisitTerminal->SetVisibility(ESlateVisibility::HitTestInvisible);
	TextBlockVisitTerminal->SetVisibility(ESlateVisibility::HitTestInvisible);
	
	SetMissionAlertText(TextToDisplay);
	
	PlayAnimation(AnimationEscapeWithPod, 0, 0);
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

void USGObjectiveToolTipWidget::DisplayCharByChar(const FString& StringToolTip)
{
	if (++CharIndex <= StringToolTip.Len())
	{
		FString StrToDisplay = StringToolTip.Mid(0, CharIndex);
		ToolTip->SetText(FText::FromString(StrToDisplay));
		
		UGameplayStatics::PlaySound2D(GetWorld(), TextClickSound);
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


