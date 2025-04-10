// Fill out your copyright notice in the Description page of Project Settings.


#include "CJPlayerController.h"
#include "CJPlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GrapplingHook.h"

void ACJPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	ACJPlayerCharacter* CurrentPlayer = GetValidPlayerCharacter();
	if (CurrentPlayer == nullptr)
		return;
	
	CurrentPlayer->GetCharacterMovement()->AirControl = 1.f;
	
}

void ACJPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(InputComponent);
	Input->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &ACJPlayerController::Move);
	Input->BindAction(JumpInputAction, ETriggerEvent::Triggered, this, &ACJPlayerController::Jump);
	Input->BindAction(InteractInputAction, ETriggerEvent::Triggered, this, &ACJPlayerController::Interact);
	Input->BindAction(LookAroundInputAction, ETriggerEvent::Triggered, this, &ACJPlayerController::LookAround);
	Input->BindAction(GrappleInputAction, ETriggerEvent::Triggered, this, &ACJPlayerController::Grapple);
	
	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	
	if (Player == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No Local Player"));
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* InputSubsytem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	if (InputSubsytem == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No Local Player Subsystem"));
		return;
	}

	if (InputMapping == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No Input Mapping"));
		return;
	}

	InputSubsytem->AddMappingContext(InputMapping, 0);
}

void ACJPlayerController::Move(const FInputActionValue& Value)
{
	ACJPlayerCharacter* CurrentPlayer = GetValidPlayerCharacter();
	if (CurrentPlayer == nullptr)
		return;

	FVector2D AxisValue2D = Value.Get<FVector2D>();
	FVector Movement = FVector(AxisValue2D.X * MoveSpeed, AxisValue2D.Y * MoveSpeed, 0);
	CurrentPlayer->AddMovementInput(CurrentPlayer->GetActorForwardVector(), Movement.Y);
	CurrentPlayer->AddMovementInput(CurrentPlayer->GetActorRightVector(), Movement.X);
}

void ACJPlayerController::Jump(const FInputActionValue& Value)
{
	ACJPlayerCharacter* CurrentPlayer = GetValidPlayerCharacter();
	if (CurrentPlayer == nullptr)
		return;
	
	bool Jump = Value.Get<bool>();
	if (Jump)
	{
		CurrentPlayer->bPressedJump = true;
	}
	else
	{
		CurrentPlayer->bPressedJump = false;
	}
	UE_LOG(LogTemp, Warning, TEXT("Jump: + %hhd"), Value.Get<bool>());
}

void ACJPlayerController::Interact(const FInputActionValue& Value)
{
	ACJPlayerCharacter* CurrentPlayer = GetValidPlayerCharacter();
	if (CurrentPlayer == nullptr)
		return;

	UE_LOG(LogTemp, Warning, TEXT("Interact: Rage = %f"), CurrentPlayer->GetRage());
}

void ACJPlayerController::LookAround(const FInputActionValue& Value)
{
	ACJPlayerCharacter* CurrentPlayer = GetValidPlayerCharacter();
	if (CurrentPlayer == nullptr)
		return;

	FVector2D AxisValue2D = Value.Get<FVector2D>();
	CurrentPlayer->AddControllerPitchInput(AxisValue2D.Y);
	CurrentPlayer->AddControllerYawInput(AxisValue2D.X);
}

void ACJPlayerController::Grapple(const FInputActionValue& Value)
{
	ACJPlayerCharacter* CurrentPlayer = GetValidPlayerCharacter();
	if (CurrentPlayer == nullptr)
		return;

	CurrentPlayer->FireGrapple();
}


ACJPlayerCharacter* ACJPlayerController::GetValidPlayerCharacter()
{
	ACJPlayerCharacter* CurrentPlayer = Cast<ACJPlayerCharacter>(GetPawn());
	if (CurrentPlayer == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ACJPlayerCharacter was nullptr"));
		return nullptr;
	}
	
	return CurrentPlayer;
}
