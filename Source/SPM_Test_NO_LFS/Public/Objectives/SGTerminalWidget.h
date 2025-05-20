// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objectives/SGGameObjectivesHandler.h"
#include "Blueprint/UserWidget.h"
#include "SGTerminalWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartMission);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCloseTerminal);
/**
 * 
 */
UCLASS()
class SPM_TEST_NO_LFS_API USGTerminalWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	FOnStartMission OnStartMission;
	FOnCloseTerminal OnCloseTerminal;
	void SetObjectiveHandler(ASGGameObjectivesHandler* ObjectiveHandler);
	void EnableStartButton();
	void DisableStartButton();
protected:
	UPROPERTY(BluePrintReadWrite, meta = (BindWidget))
	class UButton* ButtonStartMission;

	UPROPERTY(BluePrintReadWrite, meta = (BindWidget))
	class UButton* ButtonCloseTerminal;
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	USoundBase* SoundHoverButton;
	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	USoundBase* SoundPressButton;
	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	USoundBase* SoundReleaseButton;
	
	UFUNCTION()
	void OnPressStartMission();
	UFUNCTION()
	void OnReleaseStartMission();
	UFUNCTION()
	void OnClickStartMission();
	UFUNCTION()
	void OnHoverStartMission();
	UFUNCTION()
	void OnUnHoverStartMission();
	
	UFUNCTION()
	void OnPressCloseMission();
	UFUNCTION()
	void OnReleaseCloseMission();
	UFUNCTION()
	void OnClickCloseTerminal();
	UFUNCTION()
	void OnHoverCloseTerminal();
	UFUNCTION()
	void OnUnHoverCloseTerminal();

	UPROPERTY()
	class ASGGameObjectivesHandler* GameObjectivesHandler;
};
