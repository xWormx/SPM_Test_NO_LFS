#include "Player/SGPlayerController.h"
#include "Player/SGPlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "jola6902_GunsComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Gear/Grapple/SGGrapplingHook.h"
#include "Blueprint/UserWidget.h"
#include "Components/SGHealthComponent.h"
#include "Components/Counters/SGCounterComponentAmmo.h"
#include "Core/SGUpgradeSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "UI/SGMainHUD.h"

void ASGPlayerController::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = GetValidPlayerCharacter();
	if (!PlayerCharacter)
	{
		return;
	}

	/* In shipping builds, the PlayerController is created first, then the HUD, and then the PlayerCharacter.
	 * At this point, PlayerCharacter isn't null because the pawn has been spawned and possessed, but its BeginPlay hasn't yet completed.
	 * Sooo basically, we wait until the PlayerCharacter (and its components) are ready before binding the HUD to its components different events.*/
	PlayerCharacter->OnPlayerIsReady.AddLambda([this](ASGPlayerCharacter* CurrentPlayer)
	{
		ASGMainHUD* MainHUD = Cast<ASGMainHUD>(GetHUD());

		if (!MainHUD)
		{
			return;
		}

		/*if (CurrentPlayer->GrapplingHook)
		{
			MainHUD->BindToGrappleEvents(CurrentPlayer->GrapplingHook);
		}

		if (CurrentPlayer->GunsComponent)
		{
			MainHUD->BindWeaponEvents(CurrentPlayer->GunsComponent);
		}

		if (CurrentPlayer->AmmoComponent)
		{
			MainHUD->BindToAmmoEvents(CurrentPlayer->AmmoComponent);
		}*/
		MainHUD->BindToPlayerComponentEvents(CurrentPlayer);

		if (CurrentPlayer->HealthComponent)
		{
			USGHealthComponent* HealthComponent = CurrentPlayer->HealthComponent;
			HealthComponent->OnNoHealth.AddDynamic(this, &ASGPlayerController::EnableGameOver);
			HealthComponent->OnHurt.AddDynamic(this, &ASGPlayerController::PlayTempDamageEffect);
		}
	});

	//TODO: Consider moving this to SGPlayerCharacter
	if (USGUpgradeSubsystem* UpgradeSystem = GetGameInstance()->GetSubsystem<USGUpgradeSubsystem>())
	{
		const FName MovementSpeed = TEXT("MoveSpeed");
		const FName Category = TEXT("Player");
		UpgradeSystem->BindAttribute(PlayerCharacter, MovementSpeed, MovementSpeed, Category);
		if (UCharacterMovementComponent* CharacterMovement = Cast<UCharacterMovementComponent>(GetCharacter()->GetMovementComponent()))
		{
			UpgradeSystem->BindAttribute(CharacterMovement, TEXT("JumpZVelocity"), TEXT("JumpHeight"), Category);			
		}		
	}
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

	// Calle: TestButton
	Input->BindAction(TestButtonAction, ETriggerEvent::Triggered, this, &ASGPlayerController::TestButtonPressed);

	if (!ensure(GetLocalPlayer()))
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (!ensure(InputSubsystem) || !ensure(InputMapping))
	{
		return;
	}

	InputSubsystem->AddMappingContext(InputMapping, 0);
}

//---- MOVEMENT
void ASGPlayerController::Move(const FInputActionValue& Value)
{
	if (!GetValidPlayerCharacter())
	{
		return;
	}

	FVector2D AxisValue2D = Value.Get<FVector2D>();
	FVector Movement = FVector(AxisValue2D.X * PlayerCharacter->MoveSpeed, AxisValue2D.Y * PlayerCharacter->MoveSpeed, 0);

	PlayerCharacter->AddMovementInput(PlayerCharacter->GetActorForwardVector(), Movement.Y);
	PlayerCharacter->AddMovementInput(PlayerCharacter->GetActorRightVector(), Movement.X);
}

void ASGPlayerController::Jump(const FInputActionValue& Value)
{
	if (!GetValidPlayerCharacter())
	{
		return;
	}
	PlayerCharacter->bPressedJump = Value.Get<bool>();
}

void ASGPlayerController::Interact([[maybe_unused]] const FInputActionValue& Value)
{
	if (!bCanInteractWithTerminal || !GetValidPlayerCharacter())
	{
		return;
	}
	OnInteract.Broadcast();
	UE_LOG(LogTemp, Warning, TEXT("🤡Interact: Rage = %f"), PlayerCharacter->GetRage());
}

void ASGPlayerController::LookAround(const FInputActionValue& Value)
{
	if (!GetValidPlayerCharacter())
	{
		return;
	}

	FVector2D AxisValue2D = Value.Get<FVector2D>();
	PlayerCharacter->AddControllerPitchInput(AxisValue2D.Y * MouseSensitivity);
	PlayerCharacter->AddControllerYawInput(AxisValue2D.X * MouseSensitivity);
}

void ASGPlayerController::Grapple([[maybe_unused]] const FInputActionValue& Value)
{
	if (!GetValidPlayerCharacter() || !PlayerCharacter->GrapplingHook)
	{
		return;
	}

	PlayerCharacter->GrapplingHook->FireGrapple();
}

void ASGPlayerController::TestButtonPressed(const FInputActionValue& Value)
{
	OnTestButtonPressed.Broadcast();
}

//---- HELPERS
ASGPlayerCharacter* ASGPlayerController::GetValidPlayerCharacter()
{
	if (!PlayerCharacter)
	{
		PlayerCharacter = Cast<ASGPlayerCharacter>(GetPawn());
		ensure(PlayerCharacter);
	}

	return PlayerCharacter;
}

//---- TERMINAL INTERACTION
void ASGPlayerController::SetCanInteractWithTerminal(const bool bInteract)
{
	bCanInteractWithTerminal = bInteract;
}

void ASGPlayerController::SetWantToInteractWithTerminal(const bool bInteract)
{
	bWantToInteract = bInteract;
}

bool ASGPlayerController::CanInteractWithTerminal() const
{
	return bCanInteractWithTerminal;
}


//---- Added by Basir
void ASGPlayerController::PauseGame()
{
	Cast<ASGMainHUD>(GetHUD())->PauseGame();
	/*SetPause(true);
	if (!PauseMenu)
	{
		return;
	}
	PauseMenu->AddToViewport();
	bShowMouseCursor = true;
	SetInputMode(FInputModeUIOnly());*/
}

void ASGPlayerController::RestartGame()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);

	FString CurrentLevel = GetWorld()->GetMapName();
	CurrentLevel.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

	UGameplayStatics::OpenLevel(this, FName(*CurrentLevel));
}

void ASGPlayerController::EnableGameOver([[maybe_unused]] float NewHealth)
{
	
	Cast<ASGMainHUD>(GetHUD())->GameOver();
	/*SetPause(true);
	if (!GameOverMenu)
	{
		return;
	}

	GameOverMenu->AddToViewport();
	bShowMouseCursor = true;
	SetInputMode(FInputModeGameOnly());*/
}

void ASGPlayerController::PlayTempDamageEffect([[maybe_unused]] float NewHealth)
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