// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/SGGameModeBase.h"

void ASGGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("CJGameModeBase::BeginPlay"));
}