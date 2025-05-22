#include "Player/SGPlayerController.h"
#include "Player/SGPlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "jola6902_GunsComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Gear/Grapple/SGGrapplingHook.h"
#include "Gear/Weapons/SGGun.h"
#include "Blueprint/UserWidget.h"
#include "Core/SGGameInstance.h"
#include "Core/SGUpgradeSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Objectives/SGTerminalWidget.h"
#include "UI/SGMainHUD.h"

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
	
	PlayerCharacter = GetValidPlayerCharacter();
	if (PlayerCharacter == nullptr)
	{
		return;
	}

	PlayerCharacter->GetCharacterMovement()->AirControl = 1.f;

	ASGMainHUD* MainHUD = Cast<ASGMainHUD>(GetHUD());
	PlayerCharacter->OnGrapplingHookReady.AddLambda([MainHUD](ASGGrapplingHook* GrapplingHook)
	{
		if (MainHUD && GrapplingHook)
		{
			MainHUD->BindToGrappleEvents(GrapplingHook);
		}
	});

	if (USGUpgradeSubsystem* UpgradeSystem = GetGameInstance()->GetSubsystem<USGUpgradeSubsystem>())
	{
		const FName MovementSpeed = TEXT("MoveSpeed");
		const FName Category = TEXT("Player");
		UpgradeSystem->BindAttribute(this, MovementSpeed, MovementSpeed, Category);
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

void ASGPlayerController::SetCanInteractWithTerminal(const bool bInteract)
{
	bCanInteractWithTerminal = bInteract;
}

void ASGPlayerController::SetWantToInteractWithTerminal(const bool bInteract)
{
	bWantToInteract = bInteract;
}

bool ASGPlayerController::GetCanInteractWithTerminal() const
{
	return bCanInteractWithTerminal;
}

bool ASGPlayerController::GetWantToInteractWithTerminal() const
{
	return bWantToInteract;
}


void ASGPlayerController::Move(const FInputActionValue& Value)
{
	if (!GetValidPlayerCharacter())
	{
		return;
	}

	FVector2D AxisValue2D = Value.Get<FVector2D>();
	FVector Movement = FVector(AxisValue2D.X * MoveSpeed, AxisValue2D.Y * MoveSpeed, 0);

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

void ASGPlayerController::Interact(const FInputActionValue& Value)
{
	if (!bCanInteractWithTerminal || !GetValidPlayerCharacter())
	{
		return;
	}
	// Denna funktion kanske int ebehövs om vi ändå broadcastar via delegate?
	SetWantToInteractWithTerminal(true);
	OnInteract.Broadcast();
	UE_LOG(LogTemp, Warning, TEXT("Interact: Rage = %f"), PlayerCharacter->GetRage());
}

void ASGPlayerController::LookAround(const FInputActionValue& Value)
{
	if (!GetValidPlayerCharacter())
	{
		return;
	}

	FVector2D AxisValue2D = Value.Get<FVector2D>();
	PlayerCharacter->AddControllerPitchInput(AxisValue2D.Y);
	PlayerCharacter->AddControllerYawInput(AxisValue2D.X);
}

void ASGPlayerController::Grapple(const FInputActionValue& Value)
{
	if (!GetValidPlayerCharacter())
	{
		return;
	}

	PlayerCharacter->FireGrapple();
}

ASGPlayerCharacter* ASGPlayerController::GetValidPlayerCharacter()
{
	if (!PlayerCharacter)
	{
		PlayerCharacter = Cast<ASGPlayerCharacter>(GetPawn());
	}
	if (!ensure(PlayerCharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("ACJPlayerCharacter was nullptr"));
	}

	return PlayerCharacter;
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
