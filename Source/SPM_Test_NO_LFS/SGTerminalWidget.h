// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGGameObjectivesHandler.h"
#include "Blueprint/UserWidget.h"
#include "SGTerminalWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartMission);
/**
 * 
 */
UCLASS()
class SPM_TEST_NO_LFS_API USGTerminalWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	FOnStartMission OnStartMission;
	void SetObjectiveHandler(ASGGameObjectivesHandler* ObjectiveHandler);
protected:
	UPROPERTY(BluePrintReadWrite, meta = (BindWidget))
	class UButton* ButtonStartMission;
	
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnClickStartMission();
	UFUNCTION()
	void OnHoverStartMission();
	UFUNCTION()
	void OnUnHoverStartMission();

	UPROPERTY()
	class ASGGameObjectivesHandler* GameObjectivesHandler;
};
