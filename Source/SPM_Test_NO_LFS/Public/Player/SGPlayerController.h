#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "GameFramework/PlayerController.h"
#include "SGPlayerController.generated.h"

class USGTerminalWidget;
class UCameraShakeBase;
class UUserWidget;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteract);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTestButtonPressed);

class ASGPlayerCharacter;

UCLASS()
class SPM_TEST_NO_LFS_API ASGPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void SetupInputComponent() override;

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
	void TestButtonPressed(const FInputActionValue& Value);
	ASGPlayerCharacter* GetValidPlayerCharacter();

//----Start: Added by Basir
	void PauseGame();

	UFUNCTION(BlueprintCallable)
	void RestartGame();

	UFUNCTION(BlueprintCallable)
	void EnableGameOver(float NewHealth);

	UFUNCTION(BlueprintCallable)
	void PlayTempDamageEffect(float NewHealth);

	void RemoveDamageEffect();

	FTimerHandle DamageEffectTimer;
//----End: Added By Basir

	bool bCanInteractWithTerminal = false;
	bool bWantToInteract = false;

public:
	FOnInteract OnInteract;
	FOnTestButtonPressed OnTestButtonPressed;
	
	void SetCanInteractWithTerminal(const bool bInteract);
	void SetWantToInteractWithTerminal(const bool bInteract);
	bool CanInteractWithTerminal() const;

private:

	UPROPERTY()
	ASGPlayerCharacter* PlayerCharacter;

	UPROPERTY(EditAnywhere, Category = "UProperty - Player | Input ", meta = (AllowPrivateAccess = "true", ToolTip = "Mouse sensitivity for looking around. Higher values mean faster camera movement."))
	float MouseSensitivity = 0.75f;

	UPROPERTY(EditAnywhere, Category = "UProperty - Player | Input ")
	class UInputMappingContext* InputMapping;

	// INPUT
	UPROPERTY(EditDefaultsOnly, Category = "UProperty - Player | Input | Movement")
	UInputAction* MoveInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "UProperty - Player | Input | Movement")
	UInputAction* JumpInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "UProperty - Player | Input | Movement")
	UInputAction* LookAroundInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "UProperty - Player | Input | Interaction")
	UInputAction* InteractInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "UProperty - Player | Input | Interaction")
	UInputAction* GrappleInputAction;

	//----Start: Added by Basir
	UPROPERTY(EditDefaultsOnly, Category = "UProperty - Player | Input | Interaction")
	UInputAction* TestButtonAction;
	
	//----End: Added by Basir
	
//----Start: Added by Basir
	UPROPERTY(EditDefaultsOnly, Category = "UProperty - Player | Input | UI")
	UInputAction* PauseGameAction;

	UPROPERTY(EditDefaultsOnly, Category = "UProperty - Player | UI | Menus")
	UUserWidget* PauseMenu;

	UPROPERTY(EditDefaultsOnly, Category = "UProperty - Player | UI | Menus")
	UUserWidget* GameOverMenu;

	UPROPERTY(EditDefaultsOnly, Category = "UProperty - Player | UI | Hurt Effect")
	UUserWidget* TempDamageEffect;

	UPROPERTY(EditDefaultsOnly, Category = "UProperty - Player | UI | Hurt Effect")
	TSubclassOf<UCameraShakeBase> TempDamageEffectCameraShake;

	int32 ScorePoint = 0;

	UPROPERTY(EditDefaultsOnly, Category = "UProperty - Player | Score System")
	int32 KillScorePoint = 10;
//----End: Added By Basir
};
