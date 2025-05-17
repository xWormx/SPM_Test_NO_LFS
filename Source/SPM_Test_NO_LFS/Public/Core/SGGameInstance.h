// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Gear/Grapple/SGHUDGrapple.h"
#include "Objectives/SGObjectiveToolTipWidget.h"
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
	void SetHUDGrapple(USGHUDGrapple* InHUDGrapple) { HUDGrapple = InHUDGrapple; }
	void SetObjectiveTooltipWidget(USGObjectiveToolTipWidget* InObjectiveTooltipWidget) { ObjectiveToolTipWidget = InObjectiveTooltipWidget; }
	USGTerminalWidget* GetTerminalWidget() { return HUDTerminal; }
	USGHUDGrapple* GetHUDGrapple() {	return HUDGrapple; }
	USGObjectiveToolTipWidget* GetObjectiveTooltipWidget() { return ObjectiveToolTipWidget; }
	
private:
	UPROPERTY(VisibleAnywhere, Category = UPROPERTY)
	USGTerminalWidget* HUDTerminal;

	UPROPERTY(VisibleAnywhere, Category = UPROPERTY)
	USGHUDGrapple* HUDGrapple;

	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	TSubclassOf<USGObjectiveToolTipWidget> ObjectiveToolTipClass;
	
	UPROPERTY(VisibleAnywhere, Category = UPROPERTY)
	USGObjectiveToolTipWidget* ObjectiveToolTipWidget;

};
