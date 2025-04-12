// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "GameFramework/PlayerController.h"
#include "CJPlayerController.generated.h"

class ACJPlayerCharacter;
/**
 * 
 */
UCLASS()
class SPM_TEST_NO_LFS_API ACJPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void SetupInputComponent() override;

protected:
	virtual void BeginPlay() override;	
private:
	void Move(const FInputActionValue& Value);
	void Jump(const FInputActionValue& Value);
	void Interact(const FInputActionValue& Value);
	void LookAround(const FInputActionValue& Value);
	void Grapple(const FInputActionValue& Value);

	ACJPlayerCharacter* GetValidPlayerCharacter();

	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	float MoveSpeed = 5;
	
	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	class UInputMappingContext* InputMapping;

	// INPUT
	UPROPERTY(EditDefaultsOnly, Category = UPROPERTY)
	UInputAction* MoveInputAction;
	UPROPERTY(EditDefaultsOnly, Category = UPROPERTY)
	UInputAction* JumpInputAction;
	UPROPERTY(EditDefaultsOnly, Category = UPROPERTY)
	UInputAction* InteractInputAction;
	UPROPERTY(EditDefaultsOnly, Category = UPROPERTY)
	UInputAction* LookAroundInputAction;
	UPROPERTY(EditDefaultsOnly, Category = UPROPERTY)
	UInputAction* GrappleInputAction;

	// HUD
	UPROPERTY(EditDefaultsOnly, Category = UPROPERTY)
	TSubclassOf<UUserWidget> HUDClass;
	
	
};
