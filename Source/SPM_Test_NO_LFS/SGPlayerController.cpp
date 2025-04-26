// Fill out your copyright notice in the Description page of Project Settings.


#include "SGPlayerController.h"
#include "SGPlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SGGrapplingHook.h"
#include "SGGun.h"
#include "Blueprint/UserWidget.h"


void ASGPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	ASGPlayerCharacter* CurrentPlayer = GetValidPlayerCharacter();
	if (CurrentPlayer == nullptr)
		return;
	
	CurrentPlayer->GetCharacterMovement()->AirControl = 1.f;

	UUserWidget* CrossHair = CreateWidget<UUserWidget>(this, HUDClass);
	if (CrossHair)
	{
		CrossHair->AddToViewport();
	}
}

void ASGPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(InputComponent);
	Input->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &ASGPlayerController::Move);
	Input->BindAction(JumpInputAction, ETriggerEvent::Triggered, this, &ASGPlayerController::Jump);
	Input->BindAction(InteractInputAction, ETriggerEvent::Triggered, this, &ASGPlayerController::Interact);
	Input->BindAction(LookAroundInputAction, ETriggerEvent::Triggered, this, &ASGPlayerController::LookAround);
	Input->BindAction(GrappleInputAction, ETriggerEvent::Triggered, this, &ASGPlayerController::Grapple);
	Input->BindAction(FireGunInputAction, ETriggerEvent::Triggered, this, &ASGPlayerController::StartFiringGun);
	Input->BindAction(StopFireGunInputAction, ETriggerEvent::Triggered, this, &ASGPlayerController::StopFiringGun);

	Input->BindAction(PauseGameAction, ETriggerEvent::Triggered, this, &ASGPlayerController::PauseGame);  //Pause Game
	
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


void ASGPlayerController::Move(const FInputActionValue& Value)
{
	ASGPlayerCharacter* CurrentPlayer = GetValidPlayerCharacter();
	if (CurrentPlayer == nullptr)
		return;

	FVector2D AxisValue2D = Value.Get<FVector2D>();
	FVector Movement = FVector(AxisValue2D.X * MoveSpeed, AxisValue2D.Y * MoveSpeed, 0);
	CurrentPlayer->AddMovementInput(CurrentPlayer->GetActorForwardVector(), Movement.Y);
	CurrentPlayer->AddMovementInput(CurrentPlayer->GetActorRightVector(), Movement.X);
}

void ASGPlayerController::Jump(const FInputActionValue& Value)
{
	ASGPlayerCharacter* CurrentPlayer = GetValidPlayerCharacter();
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

void ASGPlayerController::Interact(const FInputActionValue& Value)
{
	if (!bCanInteractWithTerminal)
		return;

	
	ASGPlayerCharacter* CurrentPlayer = GetValidPlayerCharacter();
	if (CurrentPlayer == nullptr)
		return;
	// Denna funktion kanske int ebehövs om vi ändå broadcastar via delegate?
	SetWantToInteractWithTerminal(true);
	OnInteract.Broadcast();
	UE_LOG(LogTemp, Warning, TEXT("Interact: Rage = %f"), CurrentPlayer->GetRage());
}

void ASGPlayerController::LookAround(const FInputActionValue& Value)
{
	ASGPlayerCharacter* CurrentPlayer = GetValidPlayerCharacter();
	if (CurrentPlayer == nullptr)
		return;

	FVector2D AxisValue2D = Value.Get<FVector2D>();
	CurrentPlayer->AddControllerPitchInput(AxisValue2D.Y);
	CurrentPlayer->AddControllerYawInput(AxisValue2D.X);
}

void ASGPlayerController::Grapple(const FInputActionValue& Value)
{
	ASGPlayerCharacter* CurrentPlayer = GetValidPlayerCharacter();
	if (CurrentPlayer == nullptr)
		return;

	CurrentPlayer->FireGrapple();
}

void ASGPlayerController::StartFiringGun(const FInputActionValue& Value)
{
	if (bIsFiring) return;
	bIsFiring = true;
	FireGun();

	ASGPlayerCharacter* CurrentPlayer = GetValidPlayerCharacter();
	if (CurrentPlayer == nullptr) return;
	
	const ASGGun* Gun = CurrentPlayer->GetGunRef();
	float FireRate = Gun->GetFireRate();
	
	GetWorld()->GetTimerManager().SetTimer(FireRateTimer, this, &ASGPlayerController::FireGun, FireRate, true);
}

void ASGPlayerController::StopFiringGun(const FInputActionValue& Value)
{
	bIsFiring = false;
	GetWorld()->GetTimerManager().ClearTimer(FireRateTimer);
}

void ASGPlayerController::FireGun()
{
	ASGPlayerCharacter* CurrentPlayer = GetValidPlayerCharacter();
	if (CurrentPlayer == nullptr)
		return;
	CurrentPlayer->FireGun();
}


ASGPlayerCharacter* ASGPlayerController::GetValidPlayerCharacter()
{
	ASGPlayerCharacter* CurrentPlayer = Cast<ASGPlayerCharacter>(GetPawn());
	if (CurrentPlayer == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ACJPlayerCharacter was nullptr"));
		return nullptr;
	}
	
	return CurrentPlayer;
}

//Added by Basir 
void ASGPlayerController::PauseGame()
{

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Yellow, TEXT("Pause"));
	}
	
	SetPause(true);

	if (PauseMenu)
	{
		PauseMenu->AddToViewport();
		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());
	}
	
}
