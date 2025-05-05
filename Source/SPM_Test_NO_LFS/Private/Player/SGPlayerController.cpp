#include "Player/SGPlayerController.h"
#include "Player/SGPlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Gear/Weapons/SGGun.h"
#include "Blueprint/UserWidget.h"
#include "Core/SGUpgradeSubsystem.h"

void ASGPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	ThePlayerCharacter = GetValidPlayerCharacter();
	if (ThePlayerCharacter == nullptr)
		return;
	
	ThePlayerCharacter->GetCharacterMovement()->AirControl = 1.f;

	UUserWidget* CrossHair = CreateWidget<UUserWidget>(this, HUDClass);
	if (CrossHair)
	{
		CrossHair->AddToViewport();
	}

	bCanFire = true;

	if (USGUpgradeSubsystem* UpgradeSystem = GetGameInstance()->GetSubsystem<USGUpgradeSubsystem>())
	{
		FName PropertyName = TEXT("MoveSpeed");
		FName RowName = TEXT("MoveSpeed");
		FName Category = TEXT("Player");
		UpgradeSystem->BindAttribute(this, PropertyName, RowName, Category);		
	}
}

void ASGPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	HandleFiring(); // OK att ligga i Tick(), kollar bara 2st bools
}

void ASGPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(InputComponent);

	// Movement
	Input->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &ASGPlayerController::Move);
	Input->BindAction(JumpInputAction, ETriggerEvent::Triggered, this, &ASGPlayerController::Jump);
	Input->BindAction(LookAroundInputAction, ETriggerEvent::Triggered, this, &ASGPlayerController::LookAround);
	Input->BindAction(GrappleInputAction, ETriggerEvent::Triggered, this, &ASGPlayerController::Grapple);

	// System
	Input->BindAction(PauseGameAction, ETriggerEvent::Triggered, this, &ASGPlayerController::PauseGame);
	Input->BindAction(InteractInputAction, ETriggerEvent::Triggered, this, &ASGPlayerController::Interact);

	// Shooting
	Input->BindAction(FireGunInputAction, ETriggerEvent::Triggered, this, &ASGPlayerController::OnFireButtonPressed);
	Input->BindAction(StopFireGunInputAction, ETriggerEvent::Triggered, this, &ASGPlayerController::OnFireButtonReleased);
	Input->BindAction(SwapWeapon1InputAction, ETriggerEvent::Triggered, this, &ASGPlayerController::OnSwapWeaponKey1Pressed);
	Input->BindAction(SwapWeapon2InputAction, ETriggerEvent::Triggered, this, &ASGPlayerController::OnSwapWeaponKey2Pressed);
	Input->BindAction(SwapWeapon3InputAction, ETriggerEvent::Triggered, this, &ASGPlayerController::OnSwapWeaponKey3Pressed);
	Input->BindAction(SwapWeaponMouseWheelInputAction, ETriggerEvent::Triggered, this, &ASGPlayerController::OnSwapWeaponMouseWheel);
	
	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	
	if (LocalPlayer == nullptr)
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

void ASGPlayerController::OnFireButtonPressed(const FInputActionValue& Value)
{
	bIsFiring = true;
}

void ASGPlayerController::OnFireButtonReleased(const FInputActionValue& Value)
{
	bIsFiring = false;
}

void ASGPlayerController::HandleFiring()
{
	if (!bIsFiring || !bCanFire) return;
	FireGun();
}

void ASGPlayerController::FireGun()
{
	ASGPlayerCharacter* CurrentPlayer = GetValidPlayerCharacter();
	if (CurrentPlayer == nullptr) return;

	const ASGGun* Gun = CurrentPlayer->GetGunRef();
	if (Gun)
	{
		bCanFire = false;
		CurrentPlayer->FireGun();
		float FireRate = Gun->GetFireRate();
		GetWorld()->GetTimerManager().SetTimer(CanFireAgainTimer, this, &ASGPlayerController::CanFireAgain, FireRate, false);
	}
}

void ASGPlayerController::CanFireAgain()
{
	bCanFire = true;
}

void ASGPlayerController::OnSwapWeaponKey1Pressed(const FInputActionValue& Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("Pressed Key 1"));
	if (ThePlayerCharacter == nullptr) return;
	ThePlayerCharacter->SetCurrentGunIndex(0);
}

void ASGPlayerController::OnSwapWeaponKey2Pressed(const FInputActionValue& Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("Pressed Key 2"));
	if (ThePlayerCharacter == nullptr) return;
	ThePlayerCharacter->SetCurrentGunIndex(1);
}

void ASGPlayerController::OnSwapWeaponKey3Pressed(const FInputActionValue& Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("Pressed Key 3"));
	if (ThePlayerCharacter == nullptr) return;
	ThePlayerCharacter->SetCurrentGunIndex(2);
}

void ASGPlayerController::OnSwapWeaponMouseWheel(const FInputActionValue& Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("Mouse Scroll Wheel"));
	if (ThePlayerCharacter == nullptr) return;

	const TArray<ASGGun*>& Guns = ThePlayerCharacter->GetGuns();
	int32 NumberOfGuns = Guns.Num();
	if (NumberOfGuns == 0) return;

	int32 CurrentGunIndex = ThePlayerCharacter->GetCurrentGunIndex();
	float ScrollValue = Value.Get<float>();

	if (FMath::IsNearlyZero(ScrollValue)) return;

	if (ScrollValue > 0.f)
	{
		CurrentGunIndex = (CurrentGunIndex + 1) % NumberOfGuns;
	}
	else if (ScrollValue < 0.f)
	{
		CurrentGunIndex = (CurrentGunIndex - 1 + NumberOfGuns) % NumberOfGuns;
	}

	ThePlayerCharacter->SetCurrentGunIndex(CurrentGunIndex);
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
	SetPause(true);

	if (!PauseMenu)
	{
		return;
	}
	PauseMenu->AddToViewport();
	bShowMouseCursor = true;
	SetInputMode(FInputModeUIOnly());
}
