#pragma once

#include "CoreMinimal.h"
#include "SGPlayerController.h"
#include "GameFramework/PlayerController.h"
#include "SGDemoPlayerController.generated.h"

class USGTerminalWidget;
class UInputAction;
class USGHUDGrapple;

UCLASS()
class SPM_TEST_NO_LFS_API ASGDemoPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	FOnInteract OnInteract;

	virtual void SetupInputComponent() override;

	void SetCanInteractWithTerminal(const bool bInteract);

	void SetWantToInteractWithTerminal(const bool bInteract);

	bool CanInteractWithTerminal() const;

	bool WantToInteractWithTerminal() const;

protected:
	virtual void BeginPlay() override;

private:

	//Added by Basir
	void PauseGame();

	//Added by Basir
	UFUNCTION(BlueprintCallable)
	void RestartGame() const;

	//Added by Basir
	UFUNCTION(BlueprintCallable)
	void EnableGameOver();

	bool bCanInteractWithTerminal = false;
	bool bWantToInteract = false;

	USGHUDGrapple* GetHUDGrapple() const;

public:

	UPROPERTY(EditDefaultsOnly, Category = UPROPERTY)
	UInputAction* PauseGameAction;

	UPROPERTY(EditDefaultsOnly, Category = UPROPERTY)
	TSubclassOf<UUserWidget> HUDClass;

	UPROPERTY(EditDefaultsOnly, Category = UPROPERTY)
	UUserWidget* PauseMenu;

	UPROPERTY(EditDefaultsOnly, Category = UPROPERTY)
	UUserWidget* GameOverMenu;

	UPROPERTY(EditAnywhere, Category = UPROPERTY)
	TSubclassOf<USGTerminalWidget> HUDTerminalClass;

	UPROPERTY(VisibleAnywhere, Category = UPROPERTY)
	USGTerminalWidget* HUDTerminal;

	UPROPERTY(EditDefaultsOnly, Category = UPROPERTY)
	TSubclassOf<UUserWidget> HUDGrappleClass;

	UPROPERTY(VisibleAnywhere, Category = UPROPERTY)
	USGHUDGrapple* HUDGrapple;
};
