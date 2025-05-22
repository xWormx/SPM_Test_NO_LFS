// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "GameFramework/PlayerController.h"
#include "SGPlayerController.generated.h"

class USGTerminalWidget;
class UCameraShakeBase;
class UUserWidget;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteract);

class ASGPlayerCharacter;

UCLASS()
class SPM_TEST_NO_LFS_API ASGPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void SetupInputComponent() override;
	void SetCanInteractWithTerminal(const bool bInteract);
	void SetWantToInteractWithTerminal(const bool bInteract);
	bool GetCanInteractWithTerminal() const;
	bool GetWantToInteractWithTerminal() const;

//----Start: Added by Basir
	void UpgradeScorePoint();
	void ClearScorePoint();
	void SetScorePoint(int32 NewScorePoint);

	UFUNCTION(BlueprintCallable, Category = "Score System")
	int32 GetScorePoint() const;
//----End: Added By Basir

protected:
	virtual void BeginPlay() override;

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
	void OnSwapWeaponKeyPressed(const FInputActionInstance& Instance);
	void OnSwapWeaponMouseWheel(const FInputActionValue& Value);
	void OnReloadPressed(const FInputActionValue& Value);

	ASGPlayerCharacter* GetValidPlayerCharacter();

//----Start: Added by Basir
	void PauseGame();

	UFUNCTION(BlueprintCallable)
	void RestartGame();

	UFUNCTION(BlueprintCallable)
	void EnableGameOver();

	UFUNCTION(BlueprintCallable)
	void PlayTempDamageEffect();

	void RemoveDamageEffect();

	FTimerHandle DamageEffectTimer;
//----End: Added By Basir

	bool bCanInteractWithTerminal = false;
	bool bWantToInteract = false;


public:
	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	float MoveSpeed = 5;

	FOnInteract OnInteract;

private:

	UPROPERTY()
	ASGPlayerCharacter* PlayerCharacter;

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

//----Start: Added by Basir
	UPROPERTY(EditDefaultsOnly, Category = UPROPERTY)
	UInputAction* PauseGameAction;

	UPROPERTY(EditDefaultsOnly, Category = UPROPERTY)
	UUserWidget* PauseMenu;

	UPROPERTY(EditDefaultsOnly, Category = UPROPERTY)
	UUserWidget* GameOverMenu;

	UPROPERTY(EditDefaultsOnly, Category = UPROPERTY)
	UUserWidget* TempDamageEffect;

	UPROPERTY(EditDefaultsOnly, Category = UPROPERTY)
	TSubclassOf<UCameraShakeBase> TempDamageEffectCameraShake;

	int32 ScorePoint = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Score System")
	int32 KillScorePoint = 10;
//----End: Added By Basir
};
