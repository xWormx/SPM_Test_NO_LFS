#pragma once

#include "CoreMinimal.h"
#include "SGPlayerController.h"
#include "GameFramework/Character.h"
#include "SGDemoPlayerCharacter.generated.h"

class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class SPM_TEST_NO_LFS_API ASGDemoPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASGDemoPlayerCharacter();

protected:
	virtual void NotifyControllerChanged() override;

public:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	void Move(const FInputActionValue& Value);
	void Interact(const FInputActionValue& Value);
	void LookAround(const FInputActionValue& Value);

public:

	FOnInteract OnInteract;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "_UProperty - Movement", meta = (AllowPrivateAccess = "true"))
	float MoveSpeed = 5;

	UPROPERTY(EditDefaultsOnly, Category = "_UProperty - Movement", meta = (AllowPrivateAccess = "true"))
	float GravityScale = 2;

private:

	UPROPERTY(EditDefaultsOnly, Category = "_UProperty - Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, Category = "_UProperty - Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "_UProperty - Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "_UProperty - Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "_UProperty - Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAroundInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "_UProperty - Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> CameraComponent;
};
