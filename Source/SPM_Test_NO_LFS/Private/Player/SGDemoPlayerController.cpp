
#include "Player/SGDemoPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void ASGDemoPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void ASGDemoPlayerController::SetCanInteractWithTerminal(const bool bInteract)
{
	bCanInteractWithTerminal = bInteract;
}

void ASGDemoPlayerController::SetWantToInteractWithTerminal(const bool bInteract)
{
	bWantToInteract = bInteract;
}

bool ASGDemoPlayerController::CanInteractWithTerminal() const
{
	return bCanInteractWithTerminal;
}

bool ASGDemoPlayerController::WantToInteractWithTerminal() const
{
	return bWantToInteract;
}

void ASGDemoPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ASGDemoPlayerController::PauseGame()
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

void ASGDemoPlayerController::RestartGame() const
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);

	FString CurrentLevel = GetWorld()->GetMapName();
	CurrentLevel.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

	UGameplayStatics::OpenLevel(this, FName(*CurrentLevel));
}

void ASGDemoPlayerController::EnableGameOver()
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

USGHUDGrapple* ASGDemoPlayerController::GetHUDGrapple() const
{
	return HUDGrapple;
}

