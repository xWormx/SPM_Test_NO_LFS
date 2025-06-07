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
#include "UI/SlateWidgets/DefaultButton.h"
#include "UI/SlateWidgets/DefaultButtonGroup.h"
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
		if (PlayerCharacter)
		{
			PlayerCharacter->OnPlayerIsReady.AddUObject(this, &ASGMainHUD::BindToPlayerComponentEvents);
		}
#endif

		PostLoadMapDelegateHandle = FCoreUObjectDelegates::PostLoadMapWithWorld.AddLambda([this]([[maybe_unused]] UWorld* World)
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
		Cast<USGGameInstance>(GetGameInstance())->GetObjectiveTooltipWidget()->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (!EndGameInteractWidget.IsValid())
	{
		EndGameInteractWidget = CreateAndAddToViewPort<USGEndGameInteractWidget>(EndGameInteractClass, true);
	}

	USGObjectiveHandlerSubSystem* ObjectiveHandler = GetWorld()->GetSubsystem<USGObjectiveHandlerSubSystem>();
	if (!ObjectiveHandler->OnObjectiveStarted.IsAlreadyBound(this, &ASGMainHUD::StartDifficultyBar))
	{
			ObjectiveHandler->OnObjectiveStarted.AddDynamic(this, &ASGMainHUD::StartDifficultyBar);
	}
	if (!ObjectiveHandler->OnEndGame.IsAlreadyBound(this, &ASGMainHUD::PlayerWin))
	{
		ObjectiveHandler->OnEndGame.AddDynamic(this, &ASGMainHUD::PlayerWin);
	}

	USGGameInstance* GameInstance = GetGameInstance<USGGameInstance>();
	if (GameInstance && GameInstance->GetTerminalWidget() && !GameInstance->GetTerminalWidget()->OnVisibilityChanged.IsAlreadyBound(this, &ASGMainHUD::OnTerminalVisibilityChanged))
	{
		GetGameInstance<USGGameInstance>()->GetTerminalWidget()->OnVisibilityChanged.AddDynamic(this, &ASGMainHUD::OnTerminalVisibilityChanged);
	}
}

void ASGMainHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	USGObjectiveHandlerSubSystem* ObjectiveHandler = GetWorld()->GetSubsystem<USGObjectiveHandlerSubSystem>();
	if (ObjectiveHandler->OnObjectiveStarted.IsAlreadyBound(this, &ASGMainHUD::StartDifficultyBar))
	{
		ObjectiveHandler->OnObjectiveStarted.RemoveDynamic(this, &ASGMainHUD::StartDifficultyBar);
	}
	if (ObjectiveHandler->OnEndGame.IsAlreadyBound(this, &ASGMainHUD::PlayerWin))
	{
		ObjectiveHandler->OnEndGame.RemoveDynamic(this, &ASGMainHUD::PlayerWin);
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
		&& !PlayerCharacter->GrapplingHook->OnCanGrapple.IsAlreadyBound(MainHUDWidget->GetGrappleHookWidget(), &USGHUDGrapple::PlayValidTargetAnimation))
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
	const FText StartGameText = GetUIText("StartGameText");
	const FText LoadGameText = GetUIText("LoadGameText");
	const FText QuitGameText = GetUIText("QuitGameText");
	const FText MainMenuHeaderText = GetUIText("MainMenuHeaderText");

	auto OnStartGameClicked =  FOnClicked::CreateLambda([this]
	{
		if (USGGameInstance* GameInstance = GetGameInstance<USGGameInstance>())
		{
			GameInstance->ResetSavedGame();
			GameInstance->LoadLevel(GameLevelMap);
		}
		return FReply::Handled();
	});
	auto OnLoadGameClicked = FOnClicked::CreateLambda([this]
	{
		GetGameInstance<USGGameInstance>()->LoadLevel(GameLevelMap);
		return FReply::Handled();
	});
	auto OnQuitGameClicked = FOnClicked::CreateLambda([this]
	{
		GEngine->GameViewport->RemoveAllViewportWidgets();
		UKismetSystemLibrary::QuitGame(GetWorld(), PlayerOwner.Get(), EQuitPreference::Quit, true);
		return FReply::Handled();
	});
	
	FButtonData StartGameButton = SGWidgetFactory::MenuButton(StartGameText, OnStartGameClicked, true);
	FButtonData LoadGameButton = SGWidgetFactory::MenuButton(LoadGameText, OnLoadGameClicked);
	FButtonData QuitGameButton = SGWidgetFactory::MenuButton(QuitGameText, OnQuitGameClicked);

	FButtonGroupData StartMenuButtonGroup = SGWidgetFactory::ButtonGroup({StartGameButton,LoadGameButton,QuitGameButton});

	FMenuData StartMenuData = SGWidgetFactory::CreateMenuData(MainMenuHeaderText, StartMenuButtonGroup);

	MenuSlateWidget = SNew(SDefaultMenu).InMenuData(StartMenuData);
	if (AddToViewport)
	{
		GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(MenuWidget, SWeakWidget).PossiblyNullContent(MenuSlateWidget.ToSharedRef()));
	}
	if (GetGameInstance<USGGameInstance>()->DoesSaveGameExist())
	{
		MenuSlateWidget->GetButtonGroupWidget()->GetButtonWidget(1)->SetVisibility(EVisibility::Visible);
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
	FinalSweepObjective->OnEscapeWithPodEnabled.AddDynamic(EndGameInteractWidgetPtr,&USGEndGameInteractWidget::ShowEscapeWidget);
	FinalSweepObjective->OnEscapeWithPodDisabled.AddDynamic(EndGameInteractWidgetPtr,&USGEndGameInteractWidget::HideEscapeWidget);
}

void ASGMainHUD::InitGameMenus()
{	
	const FText ContinueGameText = GetUIText("ContinueGameText");
	const FText RestartGameText = GetUIText("RestartGameText");
	const FText LoadGameText = GetUIText("LoadGameText");
	const FText SaveGameText = GetUIText("SaveGameText");
	const FText ReturnToMainMenuText = GetUIText("ReturnToMainMenuText");
	const FText VictoryReturnToMainMenuText = GetUIText("VictoryReturnToMainMenuText");
	const FText VictoryContinueGameText = GetUIText("VictoryContinueGameText");

	const FText PausedMenuHeaderText = GetUIText("PausedMenuHeaderText");
	const FText GameOverHeaderText = GetUIText("GameOverHeaderText");
	const FText VictoryHeaderText = GetUIText("VictoryHeaderText");

	auto OnClickContinueGame = FOnClicked::CreateLambda([this]
	{
		if (MenuWidget.IsValid())
		{
			MenuWidget->SetVisibility(EVisibility::Collapsed);
		}
		EnterPlayMode();
		return FReply::Handled();
	});
	auto OnClickReturnToMainMenu = FOnClicked::CreateLambda([this]
	{
		ChangeMap(MainMenuMap);
		return FReply::Handled();
	});
	auto OnClickRestartGame =  FOnClicked::CreateLambda([this]
	{		
		ChangeMap(GameLevelMap, true);
		
		return FReply::Handled();
	});
	auto OnClickLoadSavedGame = FOnClicked::CreateLambda([this]
	{
		ChangeMap(GameLevelMap, false);
		return FReply::Handled();
	});
	auto OnClickSaveGame = FOnClicked::CreateLambda([this]
	{
		GetGameInstance<USGGameInstance>()->CollectAndSave(false);
		MenuSlateWidget->GetButtonGroupWidget()->GetButtonWidget(2)->SetEnabled(false);		
		MenuSlateWidget->GetButtonGroupWidget()->GetButtonWidget(3)->SetVisibility(EVisibility::Visible);
		return FReply::Handled();
	});
	
	FButtonData ButtonContinueGame = SGWidgetFactory::MenuButton(ContinueGameText, OnClickContinueGame, true);
	FButtonData ButtonRestartGame = SGWidgetFactory::MenuButton(RestartGameText, OnClickRestartGame);
	FButtonData ButtonLoadSavedGame = SGWidgetFactory::MenuButton(LoadGameText, OnClickLoadSavedGame);	
	FButtonData ButtonSaveGame = SGWidgetFactory::MenuButton(SaveGameText, OnClickSaveGame);
	FButtonData ButtonReturnToMainMenu = SGWidgetFactory::MenuButton(ReturnToMainMenuText,OnClickReturnToMainMenu);
	FButtonData ButtonVictoryContinueGame = SGWidgetFactory::MenuButton(VictoryContinueGameText, OnClickContinueGame, true);
	FButtonData ButtonVictoryReturnToMainMenu = SGWidgetFactory::MenuButton(VictoryReturnToMainMenuText, OnClickReturnToMainMenu);

	FButtonGroupData PauseButtonGroup = SGWidgetFactory::ButtonGroup({ButtonContinueGame, ButtonRestartGame, ButtonSaveGame, ButtonLoadSavedGame, ButtonReturnToMainMenu});
	
	FButtonGroupData GameOverButtonGroup = SGWidgetFactory::ButtonGroup({ButtonRestartGame});
	FButtonGroupData VictoryButtonGroup = SGWidgetFactory::ButtonGroup({ButtonVictoryContinueGame, ButtonVictoryReturnToMainMenu});
	
	FMenuData PauseMenuData = SGWidgetFactory::CreateMenuData(PausedMenuHeaderText,	PauseButtonGroup);
	FMenuData GameOverMenuData = SGWidgetFactory::CreateMenuData(GameOverHeaderText, GameOverButtonGroup);
	FMenuData VictoryMenuData = SGWidgetFactory::CreateMenuData(VictoryHeaderText, VictoryButtonGroup);

	MenuSlateWidget = SNew(SDefaultMenu).InMenuData(PauseMenuData);

	if (GetGameInstance<USGGameInstance>()->DoesSaveGameExist())
	{
        MenuSlateWidget->GetButtonGroupWidget()->GetButtonWidget(3)->SetVisibility(EVisibility::Visible);
	}
	
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
	TSharedPtr<SDefaultButtonWidget> Button = MenuSlateWidget->GetButtonGroupWidget()->GetButtonWidget(0);
	
	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(Button->GetButtonWidget());
	
	APlayerController* Controller = GetOwningPlayerController();
	Controller->SetInputMode(InputMode);
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
		EVisibility Visibility = GetGameInstance<USGGameInstance>()->DoesSaveGameExist() ?EVisibility::Visible: EVisibility::Collapsed;
		MenuSlateWidget->GetButtonGroupWidget()->GetButtonWidget(3)->SetVisibility(Visibility);		
		MenuSlateWidget->GetButtonGroupWidget()->GetButtonWidget(2)->SetEnabled(true);		
		MenuWidget->SetVisibility(EVisibility::Visible);
	}
	else
	{
		GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(MenuWidget, SWeakWidget).PossiblyNullContent(MenuSlateWidget.ToSharedRef()));
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
	FText VictoryText = FText::Format(FText::FromString(TEXT("{0}\n{1}")), GetUIText("VictoryHeaderText"),PlayerScore);

	FMenuData VictoryMenuData= GameMenusData[Victory];
	VictoryMenuData.HeaderElement.Text = VictoryText;
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

void ASGMainHUD::ChangeMap(const FName& LevelName, const bool bResetGame)
{
	USGGameInstance* GameInstance = GetGameInstance<USGGameInstance>();
	if (!GameInstance)
	{
		return;
	}
	
	if (bResetGame)
	{
		GameInstance->ResetSavedGame();
	}	
    GameInstance->LoadLevel(LevelName);
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
