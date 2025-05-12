#include "Player/SGDemoPlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"

ASGDemoPlayerCharacter::ASGDemoPlayerCharacter()
{
	constexpr float CameraZ = 50.f;

	PrimaryActorTick.bCanEverTick = true;
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetRelativeLocation(FVector(0, 0, CameraZ));
}

void ASGDemoPlayerCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();
	ASGPlayerController* PlayerController = Cast<ASGPlayerController>(Controller);
	if (!PlayerController)
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

	if (!Subsystem)
	{
		return;
	}
	Subsystem->AddMappingContext(InputMapping, 0);
}

void ASGDemoPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(InputComponent);

	Input->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &ASGDemoPlayerCharacter::Move);
	Input->BindAction(JumpInputAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	Input->BindAction(LookAroundInputAction, ETriggerEvent::Triggered, this, &ASGDemoPlayerCharacter::LookAround);
	Input->BindAction(InteractInputAction, ETriggerEvent::Triggered, this, &ASGDemoPlayerCharacter::Interact);

	UE_LOG(LogTemp, Warning, TEXT("SetupPlayerInputComponent"));

	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (!PlayerController)
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

	if (!Subsystem)
	{
		return;
	}
	Subsystem->AddMappingContext(InputMapping, 0);
	UE_LOG(LogTemp, Warning, TEXT("👋PLAYER: Adding mapping context"));

}

void ASGDemoPlayerCharacter::Move(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Move Pressed"));
	const FVector2D AxisValue2D = Value.Get<FVector2D>();
	UE_LOG(LogTemp, Warning, TEXT("Move Values: X=%f, Y=%f"), AxisValue2D.X, AxisValue2D.Y);
	const FVector Movement = FVector(AxisValue2D.X * MoveSpeed, AxisValue2D.Y * MoveSpeed, 0);

	AddMovementInput(GetActorForwardVector(), Movement.Y);
	AddMovementInput(GetActorRightVector(), Movement.X);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ASGDemoPlayerCharacter::Interact([[maybe_unused]] const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Interact Pressed"));
	OnInteract.Broadcast();
}

void ASGDemoPlayerCharacter::LookAround(const FInputActionValue& Value)
{
	const FVector2D AxisValue2D = Value.Get<FVector2D>();
	AddControllerPitchInput(AxisValue2D.Y);
	AddControllerYawInput(AxisValue2D.X);
}

