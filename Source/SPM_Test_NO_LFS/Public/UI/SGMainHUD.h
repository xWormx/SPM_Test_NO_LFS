#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SlateWidgets/SWidgetData/WidgetConstructionData.h"
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

	void InitHUD();
	void InitStartMenu(bool AddToViewport = true);
	void InitPauseMenu();
	void InitGameOverMenu();

	void EnterUIMode();

	UFUNCTION(BlueprintCallable, Category = "UFunction - HUD") // Tillfälligt används av BP-scriptet i PlayerController
	void EnterPlayMode();

	UFUNCTION() // Tillfälligt används för att kolla när terminalen är öppen eller stängd
	void OnTerminalVisibilityChanged(ESlateVisibility NewVisibility);

	UFUNCTION() // Tillfälligt för att kolla första gången spelaren startar ett mission
	void StartDifficultyBar();

	void PauseGame();
	void GameOver();

	void RestartGame();
	void LoadMap(const FName& LevelName);

	void ContinueGame(TSharedPtr<SWeakWidget> CallerWidget);
	void QuitGame();
	bool SaveGame();
private:
	bool IsGameLevel() const;

	static FButtonData CreateMenuButtonData(const FText& ButtonText, const FOnClicked& OnClicked);

protected:
	FDelegateHandle PostLoadMapDelegateHandle;

	//TODO: Replace with a more sustainable solution.
	//Currently only for testing - HUD should not be responsible for changing maps, and maps should not be hardcoded.
	UPROPERTY(EditAnywhere, Category ="Uproperty - HUD| Maps")
	FName GameLevelMap = FName(TEXT("ArenaLevelPlayable_v3"));

	UPROPERTY(EditAnywhere, Category ="Uproperty - HUD| Maps")
	FName MainMenuMap = FName(TEXT("MainMenuMap"));

	UPROPERTY(EditAnywhere, Category ="Uproperty - HUD")
	TSubclassOf<USGMainHUDWidget> MainHUDWidgetClass;
	
	UPROPERTY(BlueprintReadOnly, Category ="Uproperty - HUD", meta=(BindWidget))
	TWeakObjectPtr<USGMainHUDWidget> MainHUDWidget;

//------------SLATE
	TSharedPtr<SDefaultMenu> StartMenuSlateWidget;
	TSharedPtr<SWeakWidget> StartMenuWidget;

	TSharedPtr<SDefaultMenu> PauseMenuSlateWidget;
	TSharedPtr<SWeakWidget> PauseMenuWidget;

	TSharedPtr<SDefaultMenu> GameOverMenuSlateWidget;
	TSharedPtr<SWeakWidget> GameOverMenuWidget;
};
