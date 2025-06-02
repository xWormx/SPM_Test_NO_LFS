#pragma once

#include "CoreMinimal.h"
#include "Gear/Weapons/jola6902_GunsComponent.h"
#include "GameFramework/HUD.h"
#include "SlateWidgets/SWidgetData/StyleSetData.h"
#include "SlateWidgets/SWidgetData/WidgetConstructionData.h"
#include "Widgets/SWeakWidget.h"
#include "SGMainHUD.generated.h"

class SDefaultMenu;
class USGMainHUDWidget;
class ASGPlayerCharacter;

UENUM()
enum EGameMenuState : uint8
{
	Pause, GameOver, Victory
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
	
	void BindToPlayerComponentEvents(ASGPlayerCharacter* PlayerCharacter);

	void InitHUD();
	void InitStartMenu(bool AddToViewport = true);
	void InitGameMenus();

	void EnterUIMode();

	UFUNCTION(BlueprintCallable, Category = "UFunction - HUD") // Tillfälligt används av BP-scriptet i PlayerController
	void EnterPlayMode();

	UFUNCTION() // Tillfälligt används för att kolla när terminalen är öppen eller stängd
	void OnTerminalVisibilityChanged(ESlateVisibility NewVisibility);

	UFUNCTION() // Tillfälligt för att kolla första gången spelaren startar ett mission
	void StartDifficultyBar();

	void PauseGame();
	void PlayerDeath();

	void RestartGame();
	void LoadMap(const FName& LevelName);

	void ContinueGame(TSharedPtr<SWeakWidget> CallerWidget);
	void QuitGame();
	bool SaveGame();
private:
	bool IsGameLevel() const;

	static FButtonData CreateMenuButtonData(const FText& ButtonText, const FOnClicked& OnClicked, FName ButtonStyleName = StyleNames::MenuButton(), FName TextStyleName = StyleNames::MenuButtonText());
	static FTextData CreateTextData(const FText& Text, const FName StyleName);
	static FBackgroundData CreateBackgroundData(const FSlateColor& BackgroundColor, const FAlignmentData& AlignmentData);
	static FButtonGroupData CreateButtonGroupData(const TArray<FButtonData>& ButtonDataArray, const EOrientation Orientation = Orient_Vertical, const FAlignmentData&AlignmentData ={HAlign_Fill, VAlign_Top});
	static FMenuData CreateMenuData(const FTextData& TextData, const FButtonGroupData& ButtonGroupData, const FBackgroundData& BackgroundData = FBackgroundData(), FAlignmentData MenuAlignmentData = {HAlign_Fill, VAlign_Top});

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

	TSharedPtr<SDefaultMenu> GameMenuSlateWidget;
	TSharedPtr<SWeakWidget> GameMenuWidget;

	EGameMenuState LastGameMenuState = Pause;
	TMap<EGameMenuState, FMenuData> GameMenusData;
};
