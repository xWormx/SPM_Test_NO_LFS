#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Widgets/SWeakWidget.h"
#include "SGMainHUD.generated.h"

class SDefaultMenu;
class USGMainHUDWidget;
class ASGPlayerCharacter;

UCLASS()
class SPM_TEST_NO_LFS_API ASGMainHUD : public AHUD
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	template<typename T = UUserWidget>
	T* CreateAndAddToViewPort(const TSubclassOf<T>& WidgetClass, bool Add = true);
	
	void BindToPlayerComponentEvents(ASGPlayerCharacter* PlayerCharacter);
	void InitStartMenu();
	void InitPauseMenu();
	void InitGameOverMenu();

	void EnterUIState();

	UFUNCTION(BlueprintCallable, Category = "UFunction - HUD") // Tillfälligt används av BP-scriptet i PlayerController
	void EnterPlayState();

	UFUNCTION() // Tillfälligt används för att kolla när terminaln är öppen eller stängd
	void OnTerminalVisibilityChanged(ESlateVisibility NewVisibility);

	UFUNCTION() // Tillfälligt för att kolla första gången spelaren startar ett mission
	void StartDifficultyBar();

	UFUNCTION() 
	void PauseGame();
	void GameOver();

protected:
	UPROPERTY(EditAnywhere, Category ="Uproperty - HUD")
	TSubclassOf<USGMainHUDWidget> MainHUDWidgetClass;
	
	UPROPERTY(BlueprintReadOnly, Category ="Uproperty - HUD", meta=(BindWidget))
	TWeakObjectPtr<USGMainHUDWidget> MainHUDWidget;

//------------SLATE
	TSharedPtr<SDefaultMenu> DefaultSlateMenuWidget;
	TSharedPtr<SWeakWidget> DefaultMenuWidget;

	TSharedPtr<SDefaultMenu> StartMenuSlateWidget;
	TSharedPtr<SWeakWidget> StartMenuWidget;

	TSharedPtr<SDefaultMenu> PauseMenuSlateWidget;
	TSharedPtr<SWeakWidget> PauseMenuWidget;

	TSharedPtr<SDefaultMenu> GameOverMenuSlateWidget;
	TSharedPtr<SWeakWidget> GameOverMenuWidget;
};
