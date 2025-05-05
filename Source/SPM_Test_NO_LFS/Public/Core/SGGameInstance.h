// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SGGameInstance.generated.h"

class USGTerminalWidget;
/**
 * 
 */
UCLASS()
class SPM_TEST_NO_LFS_API USGGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	virtual void Init() override;
	void SetTerminalWidget(USGTerminalWidget* InWidget) { HUDTerminal = InWidget; }
	USGTerminalWidget* GetTerminalWidget() { return HUDTerminal; }
private:
	UPROPERTY(VisibleAnywhere, Category = UPROPERTY)
	USGTerminalWidget* HUDTerminal;

};
