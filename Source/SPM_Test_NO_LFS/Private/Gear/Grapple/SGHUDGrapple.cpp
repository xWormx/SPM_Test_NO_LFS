// Fill out your copyright notice in the Description page of Project Settings.


#include "Gear/Grapple/SGHUDGrapple.h"

void USGHUDGrapple::NativeConstruct()
{
	Super::NativeConstruct();
	if (ValidTargetAnimation)
	{
		
	}
}

void USGHUDGrapple::PlayValidTargetAnimation()
{
	PlayAnimation(ValidTargetAnimation);
}


void USGHUDGrapple::OnValidTargetAnimationEnd()
{
}
