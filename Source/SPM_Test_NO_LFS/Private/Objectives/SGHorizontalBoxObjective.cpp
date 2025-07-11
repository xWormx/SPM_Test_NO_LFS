// Fill out your copyright notice in the Description page of Project Settings.


#include "Objectives/SGHorizontalBoxObjective.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void USGHorizontalBoxObjective::PauseAllOngoingAnimations()
{
	if (IsAnimationPlaying(AnimationUpdateValue))
	{
		bAnimationUpdateValueWasPaused = true;
		AnimationUpdateValuePauseTime = PauseAnimation(AnimationUpdateValue);	
	}
	if (IsAnimationPlaying(AnimationKeyStartObjective))
	{
		bAnimationKeyStartObjectiveWasPaused = true;
		AnimationKeyStartObjectivePauseTime = PauseAnimation(AnimationKeyStartObjective);	
	}
	if (IsAnimationPlaying(AnimationValueCompleted))
	{
		bAnimationValueCompletedWasPaused = true;
		AnimationValueCompletedPauseTime = PauseAnimation(AnimationValueCompleted);	
	}
}

void USGHorizontalBoxObjective::ResumeAllOngoingAnimations()
{
	if (bAnimationUpdateValueWasPaused)
	{
		bAnimationUpdateValueWasPaused = false;
		PlayAnimation(AnimationUpdateValue, AnimationUpdateValuePauseTime);	
	}
	if (bAnimationKeyStartObjectiveWasPaused)
	{
		bAnimationKeyStartObjectiveWasPaused = false;
		PlayAnimation(AnimationKeyStartObjective, AnimationKeyStartObjectivePauseTime);
	}
	if (bAnimationValueCompletedWasPaused)
	{
		bAnimationValueCompletedWasPaused = false;
		PlayAnimation(AnimationValueCompleted, AnimationValueCompletedPauseTime);
	}
}

void USGHorizontalBoxObjective::SetKeyValue(FText KeyText, FText ValueText)
{
	if (TextBlockKey)
	{
		TextBlockKey->SetText(KeyText);
	}
	if (TextBlockValue)
	{
		TextBlockValue->SetText(ValueText);
	}
}

void USGHorizontalBoxObjective::SetKey(FText KeyText)
{
	if (TextBlockKey)
	{
		TextBlockKey->SetText(KeyText);
	}
}

void USGHorizontalBoxObjective::SetValue(FText ValueText)
{
	if (TextBlockValue)
	{
		TextBlockValue->SetText(ValueText);
	}
}

void USGHorizontalBoxObjective::SetKeyOpacity(float Opacity)
{
	TextBlockKey->SetOpacity(Opacity);
}

void USGHorizontalBoxObjective::SetValueOpacity(float Opacity)
{
	TextBlockValue->SetOpacity(Opacity);
}

void USGHorizontalBoxObjective::SetKeyAndValueOpacity(float Opacity)
{
	TextBlockKey->SetOpacity(Opacity);
	TextBlockValue->SetOpacity(Opacity);
}

void USGHorizontalBoxObjective::SetImage(UTexture2D* NewTexture)
{
}

void USGHorizontalBoxObjective::ShowBackground()
{
	ImageBackGround->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void USGHorizontalBoxObjective::HideBackground()
{
	ImageBackGround->SetVisibility(ESlateVisibility::Hidden);
}

void USGHorizontalBoxObjective::ShowSucceed()
{
	ImageSucceed->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void USGHorizontalBoxObjective::HideSucceed()
{
	ImageSucceed->SetVisibility(ESlateVisibility::Hidden);
}

void USGHorizontalBoxObjective::ShowFail()
{
	ImageFail->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void USGHorizontalBoxObjective::HideFail()
{
	ImageFail->SetVisibility(ESlateVisibility::Hidden);
}

void USGHorizontalBoxObjective::PlayAnimationKeyStartObjective()
{
	UE_LOG(LogTemp, Warning, TEXT("Playing Startanimation on HorizontalBox: %s"), *GetNameSafe(this));
	PlayAnimation(AnimationKeyStartObjective);
}

void USGHorizontalBoxObjective::PlayUpdateValueAnimation()
{
	PlayAnimation(AnimationUpdateValue);
}

void USGHorizontalBoxObjective::PlayAnimationValueCompleted()
{
	UE_LOG(LogTemp, Warning, TEXT("Playing Completeanimation on HorizontalBox: %s"), *GetNameSafe(this));
	PlayAnimation(AnimationValueCompleted);
}
