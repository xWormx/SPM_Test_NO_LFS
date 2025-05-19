#include "Player/SGPlayerController.h"
#include "Player/SGPlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Gear/Grapple/SGGrapplingHook.h"
#include "Gear/Weapons/SGGun.h"
#include "Blueprint/UserWidget.h"
#include "Core/SGGameInstance.h"
#include "Core/SGUpgradeSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Objectives/SGTerminalWidget.h"

void ASGPlayerController::UpgradeScorePoint()
{
	ScorePoint += KillScorePoint;
}

void ASGPlayerController::ClearScorePoint()
{
	ScorePoint = 0;
}

void ASGPlayerController::SetScorePoint(int32 NewScorePoint)
{
	ScorePoint = NewScorePoint;
}

int32 ASGPlayerController::GetScorePoint() const
{
	return ScorePoint;
}

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

	HUDGrapple = CreateWidget<USGHUDGrapple>(this, HUDGrappleClass);
	if (HUDGrapple)
	{
		Cast<USGGameInstance>(GetWorld()->GetGameInstance())->SetHUDGrapple(HUDGrapple);
		HUDGrapple->AddToViewport();
	}

	if (USGUpgradeSubsystem* UpgradeSystem = GetGameInstance()->GetSubsystem<USGUpgradeSubsystem>())
	{
		FName MovementSpeed = TEXT("MoveSpeed");
		FName Category = TEXT("Player");
		UpgradeSystem->BindAttribute(this, MovementSpeed, MovementSpeed, Category);
		if (UCharacterMovementComponent* CharacterMovement = Cast<UCharacterMovementComponent>(GetCharacter()->GetMovementComponent()))
		{
			UpgradeSystem->BindAttribute(CharacterMovement, TEXT("JumpZVelocity"), TEXT("JumpHeight"), Category);			
		}		
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
	Input->BindAction(ReloadInputAction, ETriggerEvent::Triggered, this, &ASGPlayerController::OnReloadPressed);
	
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

void ASGPlayerController::OnReloadPressed(const FInputActionValue& Value)
{
	if (ThePlayerCharacter == nullptr) return;

	UE_LOG(LogTemp, Warning, TEXT("SGPlayerController::OnReloadPressed()"));
	ThePlayerCharacter->ReloadGun();
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

void ASGPlayerController::RestartGame()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);

	FString CurrentLevel = GetWorld()->GetMapName();
	CurrentLevel.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

	UGameplayStatics::OpenLevel(this, FName(*CurrentLevel));
}

void ASGPlayerController::EnableGameOver()
{
	SetPause(true);

	if (!GameOverMenu)
	{
		return;
	}

	GameOverMenu->AddToViewport();
	bShowMouseCursor = true;
	SetInputMode(FInputModeGameOnly());
}

void ASGPlayerController::PlayTempDamageEffect()
{
	if (!TempDamageEffect)
	{
		return;
	}

	TempDamageEffect->AddToViewport();

	if (TempDamageEffectCameraShake)
	{
		PlayerCameraManager->StartCameraShake(TempDamageEffectCameraShake);
	}

	GetWorld()->GetTimerManager().SetTimer(
		DamageEffectTimer,
		this,
		&ASGPlayerController::RemoveDamageEffect,
		0.7f,
		false
		);
}

void ASGPlayerController::RemoveDamageEffect()
{
	if (TempDamageEffect)
	{
		TempDamageEffect->RemoveFromParent();
	}
}
