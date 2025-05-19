// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "GameFramework/PlayerController.h"
#include "SGPlayerController.generated.h"

class USGHUDGrapple;
class USGTerminalWidget;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteract);

class ASGPlayerCharacter;
/**
 * 
 */
UCLASS()
class SPM_TEST_NO_LFS_API ASGPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	FOnInteract OnInteract;
	
	virtual void SetupInputComponent() override;
	void SetCanInteractWithTerminal(bool bInteract) { bCanInteractWithTerminal = bInteract; }
	void SetWantToInteractWithTerminal(bool bInteract) { bWantToInteract = bInteract; }
	bool GetCanInteractWithTerminal() { return bCanInteractWithTerminal; }
	bool GetWantToInteractWithTerminal() { return bWantToInteract; }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

private:
	void Move(const FInputActionValue& Value);
	void Jump(const FInputActionValue& Value);
	void Interact(const FInputActionValue& Value);
	void LookAround(const FInputActionValue& Value);
	void Grapple(const FInputActionValue& Value);
	void OnFireButtonPressed(const FInputActionValue& Value);
	void OnFireButtonReleased(const FInputActionValue& Value);
	void HandleFiring();
	void FireGun();
	void CanFireAgain();
	void OnSwapWeaponKey1Pressed(const FInputActionValue& Value);
	void OnSwapWeaponKey2Pressed(const FInputActionValue& Value);
	void OnSwapWeaponKey3Pressed(const FInputActionValue& Value);
	void OnSwapWeaponMouseWheel(const FInputActionValue& Value);
	void OnReloadPressed(const FInputActionValue& Value);

	//Added by Basir
	void PauseGame();

	//Added by Basir
	UFUNCTION(BlueprintCallable)
	void RestartGame();

	//Added by Basir
	UFUNCTION(BlueprintCallable)
	void EnableGameOver();

	//Added by Basir
	UFUNCTION(BlueprintCallable)
	void PlayTempDamageEffect();

	void RemoveDamageEffect();

	//Added by Basir
	FTimerHandle DamageEffectTimer;
	
	FTimerHandle CanFireAgainTimer;
	bool bIsFiring = false;
	bool bCanFire;

	bool bCanInteractWithTerminal = false;
	bool bWantToInteract = false;
	
	ASGPlayerCharacter* GetValidPlayerCharacter();
	ASGPlayerCharacter* ThePlayerCharacter;

	USGHUDGrapple* GetHUDGrapple() { return HUDGrapple; }

public:
	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	float MoveSpeed = 5;
private:
	
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
	UPROPERTY(EditDefaultsOnly, Category = UPROPERTY)
	UInputAction* SwapWeapon1InputAction;
	UPROPERTY(EditDefaultsOnly, Category = UPROPERTY)
	UInputAction* SwapWeapon2InputAction;
	UPROPERTY(EditDefaultsOnly, Category = UPROPERTY)
	UInputAction* SwapWeapon3InputAction;
	UPROPERTY(EditDefaultsOnly, Category = UPROPERTY)
	UInputAction* SwapWeaponMouseWheelInputAction;
	UPROPERTY(EditDefaultsOnly, Category = UPROPERTY)
	UInputAction* ReloadInputAction;

	//Added by Basir
	UPROPERTY(EditDefaultsOnly, Category = UPROPERTY)
	UInputAction* PauseGameAction;

	// HUD
	UPROPERTY(EditDefaultsOnly, Category = UPROPERTY)
	TSubclassOf<UUserWidget> HUDClass;

	//Added by Basir
	UPROPERTY(EditDefaultsOnly, Category = UPROPERTY)
	class UUserWidget* PauseMenu;

	//Added by Basir
	UPROPERTY(EditDefaultsOnly, Category = UPROPERTY)
	class UUserWidget* GameOverMenu;

	//Added by Basir
	UPROPERTY(EditDefaultsOnly, Category = UPROPERTY)
	class UUserWidget* TempDamageEffect;

	//Added by Basir
	UPROPERTY(EditDefaultsOnly, Category = UPROPERTY)
	TSubclassOf<class UCameraShakeBase> TempDamageEffectCameraShake;

	UPROPERTY(EditDefaultsOnly, Category = UPROPERTY)
	TSubclassOf<UUserWidget> HUDGrappleClass;
	
	UPROPERTY(VisibleAnywhere, Category = UPROPERTY)
	USGHUDGrapple* HUDGrapple;
	
};
