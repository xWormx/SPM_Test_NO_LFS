// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/SGHUDGrapple.h"

void USGHUDGrapple::NativeConstruct()
{
	Super::NativeConstruct();
}

void USGHUDGrapple::PlayValidTargetAnimation()
{
	PlayAnimation(ValidTargetAnimation);
}


void USGHUDGrapple::OnValidTargetAnimationEnd()
{
}
