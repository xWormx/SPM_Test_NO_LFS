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
#include "Internationalization/StringTable.h"
#include "Internationalization/StringTableCore.h"
#include "Kismet/GameplayStatics.h"
#include "Player/SGPlayerController.h"
#include "UI/SGWidgetFactory.h"
#include "UI/SlateWidgets/DefaultMenu.h"
#include "UI/SlateWidgets/SWidgetData/StyleSetData.h"
#include "Widgets/SWeakWidget.h"
#include "UI/Widgets/SGMainHUDWidget.h"
#include "Utils/SGDeveloperSettings.h"

#include "Objectives/SGObjectiveFinalSweep.h"
#include "UI/Widgets/SGEndGameInteractWidget.h"

#define LOCTEXT_NAMESPACE "SGMainHUD"

void ASGMainHUD::BeginPlay()
{
	Super::BeginPlay();

	FStyleSetData::RefreshStyles();

	if (IsGameLevel())
	{
		InitHUD();
		InitGameMenus();
		EnterPlayMode();

#if WITH_EDITOR
		ASGPlayerCharacter* PlayerCharacter = Cast<ASGPlayerCharacter>(GetOwningPlayerController()->GetCharacter());
		PlayerCharacter->OnPlayerIsReady.AddUObject(this, &ASGMainHUD::BindToPlayerComponentEvents);
#endif

		PostLoadMapDelegateHandle = FCoreUObjectDelegates::PostLoadMapWithWorld.AddLambda(
			[this]([[maybe_unused]] UWorld* World)
			{
				ASGPlayerCharacter* PlayerCharacter = Cast<ASGPlayerCharacter>(
					GetOwningPlayerController()->GetCharacter());
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
		Cast<USGGameInstance>(GetGameInstance())->GetObjectiveTooltipWidget()->SetVisibility(
			ESlateVisibility::Collapsed);
	}

	if (!EndGameInteractWidget.IsValid())
	{
		EndGameInteractWidget = CreateAndAddToViewPort<USGEndGameInteractWidget>(EndGameInteractClass, true);
	}

	GetWorld()->GetSubsystem<USGObjectiveHandlerSubSystem>()->OnObjectiveStarted.AddDynamic(
		this, &ASGMainHUD::StartDifficultyBar);
	GetWorld()->GetSubsystem<USGObjectiveHandlerSubSystem>()->OnEndGame.AddDynamic(this, &ASGMainHUD::PlayerWin);

	Cast<USGGameInstance>(GetWorld()->GetGameInstance())->GetTerminalWidget()->OnVisibilityChanged.AddDynamic(
		this, &ASGMainHUD::OnTerminalVisibilityChanged);
}

void ASGMainHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	USGObjectiveHandlerSubSystem* ObjectiveHandler = GetWorld()->GetSubsystem<USGObjectiveHandlerSubSystem>();
	if (ObjectiveHandler->OnObjectiveStarted.IsAlreadyBound(this, &ASGMainHUD::StartDifficultyBar))
	{
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
	if (!MainHUDWidget.IsValid() || MainHUDWidget.IsStale())
	{
		InitHUD();
		return;
	}

	if (PlayerCharacter->AmmoComponent && PlayerCharacter->GunsComponent)
	{
		USGCounterComponentAmmo* AmmoComponent = PlayerCharacter->AmmoComponent;
		Ujola6902_GunsComponent* GunsComponent = PlayerCharacter->GunsComponent;

		USGWeaponsHUD* WeaponsWidgetPtr = MainHUDWidget->GetWeaponsWidget();

		if (!AmmoComponent->OnPickedUpAmmo.IsAlreadyBound(WeaponsWidgetPtr, &USGWeaponsHUD::UpdateAmmo))
		{
			AmmoComponent->OnPickedUpAmmo.AddDynamic(WeaponsWidgetPtr, &USGWeaponsHUD::UpdateAmmo);
		}

		if (!GunsComponent->OnSwitchedGun.IsAlreadyBound(WeaponsWidgetPtr, &USGWeaponsHUD::ChangeWeapon))
		{
			GunsComponent->OnSwitchedGun.AddDynamic(WeaponsWidgetPtr, &USGWeaponsHUD::ChangeWeapon);
		}

		if (!GunsComponent->OnFireGun.IsAlreadyBound(WeaponsWidgetPtr, &USGWeaponsHUD::UpdateWeapon))
		{
			GunsComponent->OnFireGun.AddDynamic(WeaponsWidgetPtr, &USGWeaponsHUD::UpdateWeapon);
		}

		if (!GunsComponent->OnReload.IsAlreadyBound(WeaponsWidgetPtr, &USGWeaponsHUD::ReloadWeapon))
		{
			GunsComponent->OnReload.AddDynamic(WeaponsWidgetPtr, &USGWeaponsHUD::ReloadWeapon);
		}

		TArray<ASGGun*> Guns = GunsComponent->GetGuns();
		WeaponsWidgetPtr->SetAvailableWeapons(Guns);
		WeaponsWidgetPtr->ChangeWeapon(0, Guns[0]);
	}

	if (PlayerCharacter->GrapplingHook
		&& !PlayerCharacter->GrapplingHook->OnCanGrapple.IsAlreadyBound(MainHUDWidget->GetGrappleHookWidget(),
		                                                                &USGHUDGrapple::PlayValidTargetAnimation))
	{
		PlayerCharacter->GrapplingHook->OnCanGrapple.AddDynamic(MainHUDWidget->GetGrappleHookWidget(),
		                                                        &USGHUDGrapple::PlayValidTargetAnimation);
	}
}

void ASGMainHUD::InitHUD()
{
	if (!MainHUDWidget.IsValid())
	{
		MainHUDWidget = CreateAndAddToViewPort<USGMainHUDWidget>(MainHUDWidgetClass);
		MainHUDWidget->PauseAndHide();
	}

	GetWorld()->GetSubsystem<USGObjectiveHandlerSubSystem>()->OnObjectiveStarted.AddDynamic(
		this, &ASGMainHUD::StartDifficultyBar);
}

void ASGMainHUD::InitStartMenu(const bool AddToViewport)
{
	// Localized text for buttons and header
	const FText StartGameText = GetUIText("StartGameText");
	const FText LoadGameText = GetUIText("LoadGameText");
	const FText QuitGameText = GetUIText("QuitGameText");
	const FText MainMenuHeaderText = GetUIText("MainMenuHeaderText");

	// Create buttons
	FButtonData ButtonDataStartGame = SGWidgetFactory::MenuButton(StartGameText, FOnClicked::CreateLambda([this]
	{
		if (USGGameInstance* GameInstance = Cast<USGGameInstance>(GetGameInstance()))
		{
			GameInstance->ResetSavedGame();
		}
		LoadMap(GameLevelMap);
		return FReply::Handled();
	}), true);
	FButtonData ButtonLoadSavedGame = SGWidgetFactory::MenuButton(LoadGameText, FOnClicked::CreateLambda([this]
	{
		LoadMap(GameLevelMap);
		return FReply::Handled();
	}));

	FButtonData ButtonDataQuitGame = SGWidgetFactory::MenuButton(QuitGameText, FOnClicked::CreateLambda([this]
	{
		QuitGame();
		return FReply::Handled();
	}));

	//Define background colors for menu
	FLinearColor StartMenuBackgroundColor = FLinearColor(0.f, 0.f, 0.f, 1.f);

	FMenuData StartMenuData = SGWidgetFactory::CreateMenuData(
		MainMenuHeaderText,
		SGWidgetFactory::ButtonGroup({ButtonDataStartGame, ButtonLoadSavedGame, ButtonDataQuitGame}, Orient_Vertical,
		                             FAlignmentData(HAlign_Fill, VAlign_Top)),
		SGWidgetFactory::Background(StartMenuBackgroundColor, FAlignmentData(HAlign_Fill, VAlign_Fill)),
		{HAlign_Fill, VAlign_Center}
	);

	// Create the Slate widget for the Start Menu (and add it to the viewport if specified)
	MenuSlateWidget = SNew(SDefaultMenu).InMenuData(StartMenuData);
	if (AddToViewport)
	{
		GEngine->GameViewport->AddViewportWidgetContent(
			SAssignNew(MenuWidget, SWeakWidget).PossiblyNullContent(MenuSlateWidget.ToSharedRef()));
	}

	//Not really necessary, but for consistency
	LastGameMenuState = Start;
	GameMenusData.Add(Start, StartMenuData);}

void ASGMainHUD::BindToEndGameInteractEvents(ASGObjectiveFinalSweep* FinalSweepObjective)
{
	if (!FinalSweepObjective)
	{
		return;
	}

	if (!EndGameInteractWidget.IsValid() || EndGameInteractWidget.IsStale())
	{
		EndGameInteractWidget = CreateAndAddToViewPort<USGEndGameInteractWidget>(EndGameInteractClass);
	}

	USGEndGameInteractWidget* EndGameInteractWidgetPtr = EndGameInteractWidget.Get();
	FinalSweepObjective->OnEscapeWithPodEnabled.AddDynamic(EndGameInteractWidgetPtr,
	                                                       &USGEndGameInteractWidget::ShowEscapeWidget);
	FinalSweepObjective->OnEscapeWithPodDisabled.AddDynamic(EndGameInteractWidgetPtr,&USGEndGameInteractWidget::HideEscapeWidget);
}

void ASGMainHUD::InitGameMenus()
{
	// Localized text for buttons and headers
	const FText ContinueGameText = GetUIText("ContinueGameText");
	const FText RestartGameText = GetUIText("RestartGameText");
	const FText LoadGameText = GetUIText("LoadGameText");
	const FText SaveGameText = GetUIText("SaveGameText");
	const FText ReturnToMainMenuText = GetUIText("ReturnToMainMenuText");

	const FText VictoryReturnToMainMenuText = GetUIText("VictoryReturnToMainMenuText");
	const FText VictoryContinueGameText = GetUIText("VictoryContinueGameText");

	const FText PausedText = GetUIText("PausedMenuHeaderText");
	const FText GameOverText = GetUIText("GameOverHeaderText");
	const FText VictoryText = GetUIText("VictoryHeaderText");

	auto OnClickContinueGame = FOnClicked::CreateLambda([this]
	                                                  {
		                                                  ContinueGame();
		                                                  return FReply::Handled();
	                                                  });
	auto OnClickReturnToMainMenu = FOnClicked::CreateLambda([this]
	{
		LoadMap(MainMenuMap);
		return FReply::Handled();
	});

	// Create buttons
	FButtonData ButtonContinueGame = SGWidgetFactory::MenuButton(ContinueGameText, OnClickContinueGame, true);
	FButtonData ButtonRestartGame = SGWidgetFactory::MenuButton(RestartGameText, FOnClicked::CreateLambda([this]
	{
		if (USGGameInstance* GameInstance = Cast<USGGameInstance>(GetGameInstance()))
		{
			GameInstance->ResetSavedGame();
		}
		RestartGame();
		return FReply::Handled();
	}));
	FButtonData ButtonLoadSavedGame = SGWidgetFactory::MenuButton(LoadGameText, FOnClicked::CreateLambda([this]
	{
		RestartGame();
		return FReply::Handled();
	}));
	FButtonData ButtonSaveGame = SGWidgetFactory::MenuButton(SaveGameText, FOnClicked::CreateLambda([this]
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
	FButtonData ButtonReturnToMainMenu = SGWidgetFactory::MenuButton(ReturnToMainMenuText,OnClickReturnToMainMenu);

	FButtonData ButtonVictoryContinueGame = SGWidgetFactory::MenuButton(VictoryContinueGameText, OnClickContinueGame, true);
	FButtonData ButtonVictoryReturnToMainMenu = SGWidgetFactory::MenuButton(VictoryReturnToMainMenuText, OnClickReturnToMainMenu);

	//Define background colors for menus
	//TODO: Replace with using styling table
	FLinearColor PausedBackgroundColor = FLinearColor(0.f, 0.f, 0.f, 0.9);
	FLinearColor GameOverBackgroundColor = FLinearColor(0.25f, 0.f, 0.f, 0.75f);

	// Collect all data for the Pause and Game Over menus
	FMenuData PauseMenuData = SGWidgetFactory::CreateMenuData(
		PausedText,
		SGWidgetFactory::ButtonGroup({
			                             ButtonContinueGame, ButtonRestartGame, ButtonSaveGame, ButtonLoadSavedGame,
			                             ButtonReturnToMainMenu
		                             },
		                             Orient_Vertical, FAlignmentData(HAlign_Fill, VAlign_Top)),
		SGWidgetFactory::Background(PausedBackgroundColor, FAlignmentData(HAlign_Fill, VAlign_Fill)),
		{HAlign_Fill, VAlign_Center}
	);

	FMenuData GameOverMenuData = SGWidgetFactory::CreateMenuData(
		GameOverText,
		SGWidgetFactory::ButtonGroup({ButtonRestartGame},
		                             Orient_Horizontal, FAlignmentData(HAlign_Fill, VAlign_Center)),
		SGWidgetFactory::Background(GameOverBackgroundColor, FAlignmentData(HAlign_Fill, VAlign_Fill)),
		{HAlign_Fill, VAlign_Center}
	);

	FMenuData VictoryMenuData = SGWidgetFactory::CreateMenuData(
		VictoryText,
		SGWidgetFactory::ButtonGroup({ButtonVictoryContinueGame, ButtonVictoryReturnToMainMenu},
		                             Orient_Horizontal, FAlignmentData(HAlign_Fill, VAlign_Fill)),
		SGWidgetFactory::Background(GameOverBackgroundColor, FAlignmentData(HAlign_Fill, VAlign_Fill)),
		{HAlign_Fill, VAlign_Center}
	);

	// Create the Slate widget
	MenuSlateWidget = SNew(SDefaultMenu).InMenuData(PauseMenuData);

	GameMenusData.Add(Pause, PauseMenuData);
	GameMenusData.Add(GameOver, GameOverMenuData);
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
	GetWorld()->GetSubsystem<USGObjectiveHandlerSubSystem>()->OnObjectiveStarted.RemoveDynamic(
		this, &ASGMainHUD::StartDifficultyBar);
}

//---- ON CLICKED

void ASGMainHUD::PauseGame()
{
	if (LastGameMenuState != Pause)
	{
		LastGameMenuState = Pause;
		MenuSlateWidget->SetMenuData(GameMenusData[Pause]);
	}
	if (MenuWidget.IsValid())
	{
		MenuWidget->SetVisibility(EVisibility::Visible);
	}
	else
	{
		GEngine->GameViewport->AddViewportWidgetContent(
			SAssignNew(MenuWidget, SWeakWidget).PossiblyNullContent(MenuSlateWidget.ToSharedRef()));
	}
	EnterUIMode();
}

void ASGMainHUD::PlayerDeath()
{
	LastGameMenuState = GameOver;

	//TODO: Add description text and remove duplicate code
	ASGPlayerController* PlayerController = Cast<ASGPlayerController>(GetOwningPlayerController());
	FText ScoreFormat = GetUIText("PlayerScore");
	FText PlayerScore = FText::Format(ScoreFormat, FText::AsNumber(PlayerController->GetScorePoint()));
	FText GameOverText = FText::Format(FText::FromString(TEXT("{0}\n{1}")), GetUIText("GameOverHeaderText"),
	                                   PlayerScore);

	FMenuData GameOverMenuData = GameMenusData[GameOver];
	GameOverMenuData.HeaderElement.Text = GameOverText;
	MenuSlateWidget->SetMenuData(GameOverMenuData);

	if (MenuWidget.IsValid())
	{
		MenuWidget->SetVisibility(EVisibility::Visible);
	}
	else
	{
		GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(MenuWidget, SWeakWidget).PossiblyNullContent(MenuSlateWidget.ToSharedRef()));
	}
	EnterUIMode();
}

void ASGMainHUD::PlayerWin()
{
	LastGameMenuState = Victory;

	//TODO: Add description text and remove duplicate code
	ASGPlayerController* PlayerController = Cast<ASGPlayerController>(GetOwningPlayerController());
	FText ScoreFormat = GetUIText("PlayerScore");
	FText PlayerScore = FText::Format(ScoreFormat, FText::AsNumber(PlayerController->GetScorePoint()));
	FText GameOverText = FText::Format(FText::FromString(TEXT("{0}\n{1}")), GetUIText("GameOverHeaderText"),PlayerScore);

	FMenuData VictoryMenuData= GameMenusData[Victory];
	VictoryMenuData.HeaderElement.Text = GameOverText;
	MenuSlateWidget->SetMenuData(VictoryMenuData);

	if (MenuWidget.IsValid())
	{
		MenuWidget->SetVisibility(EVisibility::Visible);
	}
	else
	{
		GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(MenuWidget, SWeakWidget).PossiblyNullContent(MenuSlateWidget.ToSharedRef()));
	}
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

void ASGMainHUD::ContinueGame()
{
	if (MenuWidget.IsValid())
	{
		MenuWidget->SetVisibility(EVisibility::Collapsed);
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

FText ASGMainHUD::GetUIText(const FString& Key)
{
	const UStringTable* StringTable = GetUIStringTable();
	if (StringTable)
	{
		const FStringTable& StringTableRef = StringTable->GetStringTable().Get();
		FStringTableEntryConstPtr EntryPtr = StringTableRef.FindEntry(*Key);
		if (EntryPtr.IsValid())
		{
			return FText::FromString(EntryPtr->GetSourceString());
		}
	}

	// Fallback if string not found
	EMMA_LOG(Error, TEXT("GetUIText: String with key %s not found in UI String Table"), *Key);
	return FText::Format(NSLOCTEXT("SGMainHUD", "MissingText", "Missing: {0}"), FText::FromString(Key));
}

const UStringTable* ASGMainHUD::GetUIStringTable()
{
	const USGDeveloperSettings* DevSettings = GetDefault<USGDeveloperSettings>();
	return Cast<UStringTable>(DevSettings->UIStringTable.TryLoad());
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
