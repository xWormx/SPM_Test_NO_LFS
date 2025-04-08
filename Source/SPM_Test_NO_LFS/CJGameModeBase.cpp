// Fill out your copyright notice in the Description page of Project Settings.


#include "CJGameModeBase.h"

void ACJGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("CJGameModeBase::BeginPlay"));
}