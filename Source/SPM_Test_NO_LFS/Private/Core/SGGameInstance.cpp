// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/SGGameInstance.h"

#include "SPM_Test_NO_LFS.h"
#include "Objectives/SGTerminalWidget.h"

void USGGameInstance::Init()
{
	Super::Init();

	if (ObjectiveToolTipClass)
	{
		ObjectiveToolTipWidget = Cast<USGObjectiveToolTipWidget>(CreateWidget(this, ObjectiveToolTipClass));
	}
	if (HUDTerminalClass)
	{
		HUDTerminal = Cast<USGTerminalWidget>(CreateWidget(this, HUDTerminalClass));	
	}
	
	EMMA_LOG(Warning, TEXT("Här är Emmas Log!"));
	BASIR_LOG(Warning, TEXT("Här är Basirs Log!"));
	CALLE_LOG(Warning, TEXT("Här är Calles Log!"));
	JOEL_LOG(Warning, TEXT("Här är Joels Log!"));
}
