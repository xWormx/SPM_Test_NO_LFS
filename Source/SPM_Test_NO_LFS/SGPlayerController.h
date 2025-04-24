// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "GameFramework/PlayerController.h"
#include "SGPlayerController.generated.h"

class ASGPlayerCharacter;
/**
 * 
 */
UCLASS()
class SPM_TEST_NO_LFS_API ASGPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void SetupInputComponent() override;
	void SetCanInteractWithTerminal(bool bInteract) { bCanInteractWithTerminal = bInteract; }
	void SetWantToInteractWithTerminal(bool bInteract) { bWantToInteract = bInteract; }
	bool GetCanInteractWithTerminal() { return bCanInteractWithTerminal; }
	bool GetWantToInteractWithTerminal() { return bWantToInteract; }

protected:
	virtual void BeginPlay() override;

private:
	void Move(const FInputActionValue& Value);
	void Jump(const FInputActionValue& Value);
	void Interact(const FInputActionValue& Value);
	void LookAround(const FInputActionValue& Value);
	void Grapple(const FInputActionValue& Value);
	void StartFiringGun(const FInputActionValue& Value);
	void StopFiringGun(const FInputActionValue& Value);
	void FireGun();

	//Added by Basir
	void PauseGame();
	
	FTimerHandle FireRateTimer;
	bool bIsFiring = false;

	bool bCanInteractWithTerminal = false;
	bool bWantToInteract = false;
	
	ASGPlayerCharacter* GetValidPlayerCharacter();

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
	UPROPERTY(EditDefaultsOnly, Category = UPROPERTY)
	UInputAction* FireGunInputAction;
	UPROPERTY(EditDefaultsOnly, Category = UPROPERTY)
	UInputAction* StopFireGunInputAction;

	//Added by Basir
	UPROPERTY(EditDefaultsOnly, Category = UPROPERTY)
	UInputAction* PauseGameAction;

	// HUD
	UPROPERTY(EditDefaultsOnly, Category = UPROPERTY)
	TSubclassOf<UUserWidget> HUDClass;

	//Added by Basir
	UPROPERTY(EditDefaultsOnly, Category = UPROPERTY)
	class UUserWidget* PauseMenu;
	
	
};
