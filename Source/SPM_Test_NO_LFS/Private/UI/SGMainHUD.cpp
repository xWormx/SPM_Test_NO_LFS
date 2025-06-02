#include "UI/SGMainHUD.h"

#include "Gear/Weapons/jola6902_GunsComponent.h"
#include "SPM_Test_NO_LFS.h"
#include "Components/Counters/SGCounterComponentAmmo.h"
#include "Core/SGObjectiveHandlerSubSystem.h"
#include "UI/Widgets/SGHUDGrapple.h"
#include "Gear/Grapple/SGGrapplingHook.h"
#include "UI/Widgets/SGWeaponsHUD.h"
#include "Gear/Weapons/SGGun.h"
#include "Objectives/SGTerminal.h"
#include "Components/Widget.h"
#include "Core/SGGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Player/SGPlayerController.h"
#include "UI/SlateWidgets/DefaultMenu.h"
#include "UI/SlateWidgets/SWidgetData/StyleSetData.h"
#include "Widgets/SWeakWidget.h"
#include "UI/Widgets/SGMainHUDWidget.h"

#define LOCTEXT_NAMESPACE "SGMainHUD"

void ASGMainHUD::BeginPlay()
{
	Super::BeginPlay();

	FStyleSetData::Initialize();

	if (IsGameLevel())
	{
		InitHUD();
		InitGameMenus();
		/*InitPauseMenu();
		InitGameOverMenu();*/

		EnterPlayMode();

		PostLoadMapDelegateHandle = FCoreUObjectDelegates::PostLoadMapWithWorld.AddLambda([this](UWorld* World)
		{
			ASGPlayerCharacter* PlayerCharacter = Cast<ASGPlayerCharacter>(GetOwningPlayerController()->GetCharacter());
			BindToPlayerComponentEvents(PlayerCharacter);
		});
	}
	else
	{
		InitStartMenu();

		EnterUIMode();
		/* SetVisibility is called manually here because:
		 * 1. ObjectiveTooltipWidget is not attached to the MainHUDWidget hierarchy
		 * 2. EnterUIState() cannot handle this widget since MainHUDWidget is not created yet when we are in the start menu*/
		Cast<USGGameInstance>(GetGameInstance())->GetObjectiveTooltipWidget()->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ASGMainHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	USGObjectiveHandlerSubSystem* ObjectiveHandler = GetWorld()->GetSubsystem<USGObjectiveHandlerSubSystem>();
	if (ObjectiveHandler->OnObjectiveStarted.IsAlreadyBound(this, &ASGMainHUD::StartDifficultyBar))
	{
		EMMA_LOG(Warning, TEXT("ASGMainHUD::EndPlay: Unbinding OnObjectiveStarted from StartDifficultyBar"));
		ObjectiveHandler->OnObjectiveStarted.RemoveDynamic(this, &ASGMainHUD::StartDifficultyBar);
	}
	FCoreUObjectDelegates::PostLoadMapWithWorld.Remove(PostLoadMapDelegateHandle);
	Super::EndPlay(EndPlayReason);
}

//---- INITS

void ASGMainHUD::BindToPlayerComponentEvents(ASGPlayerCharacter* PlayerCharacter)
{
	if (!IsGameLevel() || !PlayerCharacter)
	{
		return;
	}
	
	if (PlayerCharacter->AmmoComponent && PlayerCharacter->GunsComponent)
	{
		USGCounterComponentAmmo* AmmoComponent = PlayerCharacter->AmmoComponent;
		Ujola6902_GunsComponent* GunsComponent = PlayerCharacter->GunsComponent;
		
		USGWeaponsHUD* WeaponsWidgetPtr = MainHUDWidget->GetWeaponsWidget();
		
		AmmoComponent->OnPickedUpAmmo.AddDynamic(WeaponsWidgetPtr, &USGWeaponsHUD::UpdateAmmo);

		GunsComponent->OnSwitchedGun.AddDynamic(WeaponsWidgetPtr, &USGWeaponsHUD::ChangeWeapon);
		GunsComponent->OnFireGun.AddDynamic(WeaponsWidgetPtr, &USGWeaponsHUD::UpdateWeapon);
		GunsComponent->OnReload.AddDynamic(WeaponsWidgetPtr, &USGWeaponsHUD::UpdateWeapon);

		TArray<ASGGun*> Guns = GunsComponent->GetGuns();
		WeaponsWidgetPtr->SetAvailableWeapons(Guns);
		WeaponsWidgetPtr->ChangeWeapon(0, Guns[0]);		
	}
	
	if (PlayerCharacter->GrapplingHook)
	{
		PlayerCharacter->GrapplingHook->OnCanGrapple.AddDynamic(MainHUDWidget->GetGrappleHookWidget(), &USGHUDGrapple::PlayValidTargetAnimation);
	}
}

void ASGMainHUD::InitHUD()
{
	if (!MainHUDWidget.IsValid())
	{
		MainHUDWidget = CreateAndAddToViewPort<USGMainHUDWidget>(MainHUDWidgetClass);
		MainHUDWidget->PauseAndHide();
	}

	GetWorld()->GetSubsystem<USGObjectiveHandlerSubSystem>()->OnObjectiveStarted.AddDynamic(this, &ASGMainHUD::StartDifficultyBar);
}

void ASGMainHUD::InitStartMenu(const bool AddToViewport)
{
	// Localized text for buttons and header
	const FText StartGameText = LOCTEXT("StartGameText", "Start Game");
	const FText QuitGameText = LOCTEXT("QuitGameText", "Quit Game");
	const FText MainMenuHeaderText = LOCTEXT("MainMenuHeaderText", "Main Menu");

	// Create buttons
	FButtonData ButtonDataStartGame = CreateMenuButtonData(StartGameText,FOnClicked::CreateLambda([this]
		{
			LoadMap(GameLevelMap);
			return FReply::Handled();
		}), StyleNames::MenuPrimaryButton(), StyleNames::MenuPrimaryButtonText());
	FButtonData ButtonDataQuitGame = CreateMenuButtonData(QuitGameText,FOnClicked::CreateLambda([this]
		{
			QuitGame();
			return FReply::Handled();
		}));

	//Define background colors for menu
	FLinearColor StartMenuBackgroundColor = FLinearColor(0.f, 0.f, 0.f, 0.8f);

	FMenuData MenuData = CreateMenuData(
		CreateTextData(MainMenuHeaderText, StyleNames::MenuHeaderText()),
		CreateButtonGroupData({ ButtonDataStartGame, ButtonDataQuitGame }, Orient_Vertical, FAlignmentData(HAlign_Fill, VAlign_Top)),
		CreateBackgroundData(StartMenuBackgroundColor, FAlignmentData(HAlign_Fill, VAlign_Fill)),
	{HAlign_Fill, VAlign_Center}
	);

	// Create the Slate widget for the Start Menu (and add it to the viewport if specified)
	StartMenuSlateWidget = SNew(SDefaultMenu).InMenuData(MenuData);
	if (AddToViewport)
	{
		GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(StartMenuWidget, SWeakWidget).PossiblyNullContent(StartMenuSlateWidget.ToSharedRef()));
	}
}

//TODO: Endgame meny (för när man vinner som visar mängden kills + restart + quit game) (Emma fixar)
void ASGMainHUD::InitGameMenus()
{
	// Localized text for buttons and headers
	const FText ContinueGameText = LOCTEXT("ContinueGameText", "Continue Game");
	const FText RestartGameText = LOCTEXT("RestartGameText", "Restart Game");
	const FText SaveGameText = LOCTEXT("SaveGameText", "Save Game");
	const FText ReturnToMainMenuText = LOCTEXT("ReturnToMainMenuText", "Return to Main Menu");

	const FText PausedText = LOCTEXT("PausedMenuHeaderText", "Paused");
	const FText GameOverText = LOCTEXT("GameOverHeaderText", "Game Over");
	const FText VictoryText = LOCTEXT("VictoryHeaderText", "Victory");
	// Create buttons
	FButtonData ButtonContinueGame = CreateMenuButtonData(ContinueGameText,FOnClicked::CreateLambda([this]
		{
			ContinueGame(PauseMenuWidget);
			return FReply::Handled();
		}), StyleNames::MenuPrimaryButton(), StyleNames::MenuPrimaryButtonText());
	FButtonData ButtonRestartGame = CreateMenuButtonData(RestartGameText,FOnClicked::CreateLambda([this]
		{
			RestartGame();
			return FReply::Handled();
		}));
	FButtonData ButtonSaveGame = CreateMenuButtonData(SaveGameText,FOnClicked::CreateLambda([this]
		{
			bool bSaved = SaveGame();
			EMMA_LOG(Warning, TEXT("❤️TODO: Add feedback for: Saving game!"));
			if (bSaved)
			{
				EMMA_LOG(Warning, TEXT("❤️TODO: Add feedback for: Saved game!"));
			}
			else
			{
				EMMA_LOG(Warning, TEXT("❤️TODO: Add feedback for: Failed to Save Game!"));
			}
			return FReply::Handled();
		}));
	FButtonData ButtonReturnToMainMenu = CreateMenuButtonData(ReturnToMainMenuText,FOnClicked::CreateLambda([this]
		{
			LoadMap(MainMenuMap);
			return FReply::Handled();
		}));

	//Define background colors for menus
	FLinearColor PausedBackgroundColor = FLinearColor(0.f, 0.f, 0.f, 0.9f);
	FLinearColor GameOverBackgroundColor = FLinearColor(0.25f, 0.f, 0.f, 0.75f);

	// Collect all data for the Pause and Game Over menus
	FMenuData PauseMenuData = CreateMenuData(
		CreateTextData(PausedText, StyleNames::MenuHeaderText()),
		CreateButtonGroupData({ ButtonContinueGame, ButtonRestartGame, ButtonSaveGame, ButtonReturnToMainMenu },
			Orient_Vertical, FAlignmentData(HAlign_Fill, VAlign_Top)),
		CreateBackgroundData(PausedBackgroundColor, FAlignmentData(HAlign_Fill, VAlign_Fill)),
{HAlign_Fill, VAlign_Center}
	);

	FMenuData GameOverMenuData = CreateMenuData(
		CreateTextData(GameOverText, StyleNames::MenuHeaderText()),
		CreateButtonGroupData({ButtonRestartGame },
			Orient_Horizontal, FAlignmentData(HAlign_Fill, VAlign_Fill)),
		CreateBackgroundData(GameOverBackgroundColor, FAlignmentData(HAlign_Fill, VAlign_Fill)),
		{HAlign_Fill, VAlign_Center}
	);

	FMenuData VictoryMenuData = CreateMenuData(
		CreateTextData(VictoryText, StyleNames::MenuHeaderText()),
		CreateButtonGroupData({ButtonContinueGame,ButtonReturnToMainMenu},
			Orient_Horizontal, FAlignmentData(HAlign_Fill, VAlign_Fill)),
		CreateBackgroundData(GameOverBackgroundColor, FAlignmentData(HAlign_Fill, VAlign_Fill)),
		{HAlign_Fill, VAlign_Center}
	);

	// Create the Slate widgets for Pause and Game Over menus
	PauseMenuSlateWidget = SNew(SDefaultMenu).InMenuData(PauseMenuData);
	GameOverMenuSlateWidget = SNew(SDefaultMenu).InMenuData(GameOverMenuData);

	GameMenuSlateWidget = SNew(SDefaultMenu).InMenuData(PauseMenuData);
	GameMenusData.Add(Pause, PauseMenuData);
	GameMenusData.Add( GameOver, GameOverMenuData);
	GameMenusData.Add(Victory, VictoryMenuData);

}

//---- UI STATE MANAGEMENT

void ASGMainHUD::EnterUIMode()
{
	if (MainHUDWidget.IsValid())
	{
		MainHUDWidget->PauseAndHide();
		MainHUDWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	APlayerController* Controller = GetOwningPlayerController();
	Controller->SetInputMode(FInputModeUIOnly());
	Controller->bShowMouseCursor = true;
	Controller->SetPause(true);
}

void ASGMainHUD::EnterPlayMode()
{
	if (MainHUDWidget.IsValid())
	{
		MainHUDWidget->SetVisibility(ESlateVisibility::Visible);
		MainHUDWidget->PlayAndShow();
	}

	APlayerController* Controller = GetOwningPlayerController();
	Controller->SetInputMode(FInputModeGameOnly());
	Controller->bShowMouseCursor = false;
	Controller->SetPause(false);
}

//---- GAME EVENTS
void ASGMainHUD::OnTerminalVisibilityChanged(ESlateVisibility NewVisibility)
{
	if (NewVisibility == ESlateVisibility::Visible)
	{
		EnterUIMode();
	}
	else if (NewVisibility == ESlateVisibility::Hidden || NewVisibility == ESlateVisibility::Collapsed)
	{
		EnterPlayMode();
	}
}

void ASGMainHUD::StartDifficultyBar()
{
	MainHUDWidget->StartDifficultyBar();
	GetWorld()->GetSubsystem<USGObjectiveHandlerSubSystem>()->OnObjectiveStarted.RemoveDynamic(this, &ASGMainHUD::StartDifficultyBar);
}

//---- ON CLICKED EVENTS

void ASGMainHUD::PauseGame()
{
	if (LastGameMenuState != Pause)
	{
		LastGameMenuState = Pause;
		GameMenuSlateWidget->SetMenuData(GameMenusData[Pause]);
	}
	if (GameMenuWidget.IsValid())
	{
		GameMenuWidget->SetVisibility(EVisibility::Visible);
	}
	else
	{
		GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(GameMenuWidget, SWeakWidget).PossiblyNullContent(GameMenuSlateWidget.ToSharedRef()));
	}
	/*if (PauseMenuWidget.IsValid())
	{
		PauseMenuWidget->SetVisibility(EVisibility::Visible);
	}
	else
	{
		GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(PauseMenuWidget, SWeakWidget).PossiblyNullContent(PauseMenuSlateWidget.ToSharedRef()));
	}*/
	EnterUIMode();
}

void ASGMainHUD::PlayerDeath()
{
	LastGameMenuState = GameOver;

	ASGPlayerController* PlayerController = Cast<ASGPlayerController>(GetOwningPlayerController());

	FText ScoreFormat = LOCTEXT("PlayerScore", "You scored: {0}");
	FText PlayerScore = FText::Format(ScoreFormat, FText::AsNumber(PlayerController->GetScorePoint()));
	FText GameOverText = FText::Format(FText::FromString(TEXT("{0}\n{1}")), LOCTEXT("GameOverHeaderText", "Game Over"), PlayerScore);

	FMenuData GameOverMenuData = GameMenusData[GameOver];
	GameOverMenuData.HeaderElement = CreateTextData(GameOverText, StyleNames::MenuHeaderText());
	GameMenuSlateWidget->SetMenuData(GameOverMenuData);

	if (GameMenuWidget.IsValid())
	{
		GameMenuWidget->SetVisibility(EVisibility::Visible);
	}
	else
	{
		GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(GameMenuWidget, SWeakWidget).PossiblyNullContent(GameMenuSlateWidget.ToSharedRef()));
	}

//	GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(GameOverMenuWidget, SWeakWidget).PossiblyNullContent(GameOverMenuSlateWidget.ToSharedRef()));
	EnterUIMode();
}

void ASGMainHUD::RestartGame()
{
	FString CurrentLevel = GetWorld()->GetMapName();
	CurrentLevel.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
	LoadMap(FName(*CurrentLevel));
}

void ASGMainHUD::LoadMap(const FName& LevelName)
{
	GEngine->GameViewport->RemoveAllViewportWidgets();
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	UGameplayStatics::OpenLevel(this, LevelName);
}

void ASGMainHUD::ContinueGame([[maybe_unused]] TSharedPtr<SWeakWidget> CallerWidget)
{
	//CallerWidget->SetVisibility(EVisibility::Collapsed);
	if (GameMenuWidget.IsValid())
	{
		GameMenuWidget->SetVisibility(EVisibility::Collapsed);
	}
	EnterPlayMode();
}

void ASGMainHUD::QuitGame()
{
	GEngine->GameViewport->RemoveAllViewportWidgets();
	UKismetSystemLibrary::QuitGame(GetWorld(), PlayerOwner.Get(), EQuitPreference::Quit, true);
}

bool ASGMainHUD::SaveGame()
{
	bool bAsync = false;
	GetGameInstance<USGGameInstance>()->CollectAndSave(bAsync);
	return true; //TODO: Actually return if the save was successful or not
}

//----HELPERS
bool ASGMainHUD::IsGameLevel() const
{
	const FString CurrentLevel = GetWorld()->GetMapName();
	return CurrentLevel.Contains(GameLevelMap.ToString());
}

FButtonData ASGMainHUD::CreateMenuButtonData(const FText& ButtonText, const FOnClicked& OnClicked, const FName ButtonStyleName, const FName TextStyleName)
{
	FButtonData Button = FButtonData();
	Button.TextData.Text = ButtonText;
	Button.TextData.TextStyle = FStyleSetData::Get().GetWidgetStyle<FTextBlockStyle>(TextStyleName);
	Button.ButtonStyle = FStyleSetData::Get().GetWidgetStyle<FButtonStyle>(ButtonStyleName);
	Button.OnClicked = OnClicked;

	return Button;
}

FTextData ASGMainHUD::CreateTextData(const FText& Text, const FName StyleName)
{
	FTextData TextData;
	TextData.Text = Text;
	TextData.TextStyle = FStyleSetData::Get().GetWidgetStyle<FTextBlockStyle>(StyleName);
	return TextData;
}

FBackgroundData ASGMainHUD::CreateBackgroundData(const FSlateColor& BackgroundColor, const FAlignmentData& AlignmentData)
{
	FBackgroundData BackgroundData;
	BackgroundData.BackgroundColor = BackgroundColor;
	BackgroundData.BackgroundAlignment = AlignmentData;
	return BackgroundData;
}

FButtonGroupData ASGMainHUD::CreateButtonGroupData(const TArray<FButtonData>& ButtonDataArray, const EOrientation Orientation, const FAlignmentData& AlignmentData)
{
	FButtonGroupData ButtonGroupData;
	ButtonGroupData.ButtonDataArray = ButtonDataArray;
	ButtonGroupData.Orientation = Orientation;
	ButtonGroupData.Alignment = AlignmentData;
	return ButtonGroupData;
}

FMenuData ASGMainHUD::CreateMenuData(const FTextData& TextData, const FButtonGroupData& ButtonGroupData, const FBackgroundData& BackgroundData, const FAlignmentData MenuAlignmentData)
{
	FMenuData MenuData;
	MenuData.HeaderElement = TextData;
	MenuData.ButtonGroupElement = ButtonGroupData;
	MenuData.BackgroundData = BackgroundData;
	MenuData.ElementAlignment = MenuAlignmentData;
	return MenuData;
}

template <typename T>
T* ASGMainHUD::CreateAndAddToViewPort(const TSubclassOf<T>& WidgetClass, const bool Add)
{
	if (!WidgetClass)
	{
		return nullptr;
	}

	T* Widget = CreateWidget<T>(GetWorld(), WidgetClass);

	if (!Widget)
	{
		return nullptr;
	}

	if (Add)
	{
		Widget->AddToViewport();
	}

	return Widget;
}

#undef LOCTEXT_NAMESPACE
