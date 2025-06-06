#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SlateWidgets/SWidgetData/WidgetConstructionData.h"
#include "Widgets/SWeakWidget.h"
#include "SGMainHUD.generated.h"

class ASGObjectiveFinalSweep;
class USGEndGameInteractWidget;
class SDefaultMenu;
class USGMainHUDWidget;
class ASGPlayerCharacter;

UENUM(BlueprintType)
enum EGameMenuState : uint8
{
	Start, Pause, GameOver, Victory
};

UCLASS()
class SPM_TEST_NO_LFS_API ASGMainHUD : public AHUD
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	template<typename T = UUserWidget>
	T* CreateAndAddToViewPort(const TSubclassOf<T>& WidgetClass, bool Add = true);


	
protected:
	//----INITIALIZATION
	void InitHUD();
	void InitStartMenu(bool AddToViewport = true);
	void InitGameMenus();
	
public:
//----BINDING & HANDLING	
	void BindToPlayerComponentEvents(ASGPlayerCharacter* PlayerCharacter);
	void BindToEndGameInteractEvents(ASGObjectiveFinalSweep* FinalSweepObjective);

protected:
	UFUNCTION() 
	void OnTerminalVisibilityChanged(ESlateVisibility NewVisibility);

	UFUNCTION()
	void StartDifficultyBar();

	UFUNCTION()
	void PlayerWin();

public:
	void PauseGame();
	void PlayerDeath();
	
private:
	void ChangeMap(const FName& LevelName, bool bResetGame = false);
	void EnterUIMode();	
    void EnterPlayMode();

	bool IsGameLevel() const;
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

	//----ENDGAME INTERACT
	UPROPERTY(EditAnywhere, Category ="Uproperty - HUD")
	TSubclassOf<USGEndGameInteractWidget> EndGameInteractClass;

	UPROPERTY(BlueprintReadOnly, Category ="Uproperty - HUD", meta=(BindWidget))
	TWeakObjectPtr<USGEndGameInteractWidget> EndGameInteractWidget;
	
	UPROPERTY(BlueprintReadOnly, Category ="Uproperty - HUD", meta=(BindWidget))
	TWeakObjectPtr<USGMainHUDWidget> MainHUDWidget;

//------------SLATE
	TSharedPtr<SDefaultMenu> MenuSlateWidget;
	TSharedPtr<SWeakWidget> MenuWidget;

	EGameMenuState LastGameMenuState = Pause;
	TMap<EGameMenuState, FMenuData> GameMenusData;

//------------STRING TABLE

	// Gets text from the UI String Table in developer settings
	static FText GetUIText(const FString& Key);

	// Gets the UI String Table from developer settings
	static const UStringTable* GetUIStringTable();
};
