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
	NewHorizontalBox->HideFail();
	NewHorizontalBox->HideSucceed();
	
	// Returnerar Sloten så vi kan sätta allignment och padding på den.
	UVerticalBoxSlot* VerticalBoxSlot = VerticalBoxMission->AddChildToVerticalBox(NewHorizontalBox);
	VerticalBoxSlot->SetPadding(VerticalPadding);
	VerticalBoxSlot->SetHorizontalAlignment(HAlign_Fill);
	VerticalBoxSlot->SetVerticalAlignment(VAlign_Fill);

	HorizontalObjectiveList.Add(NewHorizontalBox);
	
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
	PlayAnimation(MoveToolTipToProgressWindow, 0, 1);
	//ToolTip->SetText(NewToolTip);
	bIsHidden = false;
	GetWorld()->GetTimerManager().SetTimer(CharByCharTimer, FTimerDelegate::CreateLambda([this, NewToolTip]()
	{
		DisplayCharByChar(NewToolTip.ToString());	
	}), 0.05f, true);
}

void USGObjectiveToolTipWidget::DisplayTimer(FText NewTimerText)
{
	ScaleBoxTimer->SetVisibility(ESlateVisibility::HitTestInvisible);
	TextTimer->SetText(NewTimerText);
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
	DifficultyBarOffsetLeft += 100*InDeltaTime;
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

void USGObjectiveToolTipWidget::Hide()
{
	CurrentOpacity = 1.0f;
	bHasFadedOut = true;
	bShouldRender = false;
	bIsHidden = true;
}

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
	//ScaleBoxMission->SetVisibility(ESlateVisibility::HitTestInvisible);
}


