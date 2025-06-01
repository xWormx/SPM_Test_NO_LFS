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
#include "UI/SlateWidgets/DefaultMenu.h"
#include "UI/SlateWidgets/SWidgetData/StyleSetData.h"
#include "Widgets/SWeakWidget.h"
#include "UI/Widgets/SGMainHUDWidget.h"

void ASGMainHUD::BeginPlay()
{
	Super::BeginPlay();

	FStyleSetData::Initialize();

	if (IsGameLevel())
	{
		InitHUD();
		InitPauseMenu();
		InitGameOverMenu();

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
	FButtonData ButtonDataStartGame = CreateMenuButtonData(
		FText::FromString("Start Game"),
		FOnClicked::CreateLambda([this]
		{
			LoadMap(GameLevelMap);
			return FReply::Handled();
		})
	);
	FButtonData ButtonDataQuitGame = CreateMenuButtonData(
		FText::FromString("Quit Game"),
		FOnClicked::CreateLambda([this]
		{
			QuitGame();
			return FReply::Handled();
		})
	);

	FMenuData MenuData;
	MenuData.TextData.Text = FText::FromString("Main Menu");
	MenuData.TextData.TextStyle = FStyleSetData::Get().GetWidgetStyle<FTextBlockStyle>(StyleNames::MenuHeaderText());

	MenuData.ButtonGroupData.ButtonDataArray = { ButtonDataStartGame, ButtonDataQuitGame };
	MenuData.ButtonGroupData.ButtonGroupOrientation = Orient_Vertical;
	MenuData.BackgroundColor = FLinearColor(0.f, 0.f, 0.f, 0.8f);
	MenuData.MenuAlignmentData = FAlignmentData(HAlign_Center, VAlign_Center);

	StartMenuSlateWidget = SNew(SDefaultMenu).InMenuData(MenuData);
	if (AddToViewport)
	{
		GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(StartMenuWidget, SWeakWidget).PossiblyNullContent(StartMenuSlateWidget.ToSharedRef()));
	}
}

void ASGMainHUD::InitPauseMenu()
{
	FButtonData ButtonContinueGame = CreateMenuButtonData(
		FText::FromString("Continue Game"),
		FOnClicked::CreateLambda([this]
		{
			ContinueGame(PauseMenuWidget);
			return FReply::Handled();
		})
	);

	FButtonData ButtonRestartGame = CreateMenuButtonData(
		FText::FromString("Restart Game"),
		FOnClicked::CreateLambda([this]
		{
			RestartGame();
			return FReply::Handled();
		})
	);

	FButtonData ButtonSaveGame = CreateMenuButtonData(
		FText::FromString("Save Game"),
		FOnClicked::CreateLambda([this]
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
		})
	);

	FButtonData ButtonReturnToMainMenu = CreateMenuButtonData(
		FText::FromString("Return to Main Menu"),
		FOnClicked::CreateLambda([this]
		{
			LoadMap(MainMenuMap);
			return FReply::Handled();
		})
	);

	FMenuData MenuData;
	MenuData.TextData.Text = FText::FromString("Paused");
	MenuData.TextData.TextStyle = FStyleSetData::Get().GetWidgetStyle<FTextBlockStyle>(StyleNames::MenuHeaderText());

	MenuData.BackgroundColor = FLinearColor(0.f, 0.f, 0.f, 0.9f);
	MenuData.ButtonGroupData.ButtonDataArray = { ButtonContinueGame, ButtonRestartGame, ButtonSaveGame, ButtonReturnToMainMenu };
	MenuData.ButtonGroupData.ButtonGroupOrientation = Orient_Vertical;
	MenuData.MenuAlignmentData = FAlignmentData(HAlign_Center, VAlign_Center);
	PauseMenuSlateWidget = SNew(SDefaultMenu).InMenuData(MenuData);	
}

void ASGMainHUD::InitGameOverMenu()
{
	FButtonData ButtonRestartGame = CreateMenuButtonData(
		FText::FromString("Return To Main Menu"),
		FOnClicked::CreateLambda([this]
		{
			LoadMap(MainMenuMap);
			return FReply::Handled();
		})
	);

	//TODO: Add optional description text to MenuData
	FMenuData MenuData;
	MenuData.TextData.Text = FText::FromString("Game Over");
	MenuData.TextData.TextStyle = FStyleSetData::Get().GetWidgetStyle<FTextBlockStyle>(StyleNames::MenuHeaderText());
	MenuData.ButtonGroupData.ButtonDataArray = { ButtonRestartGame };
	MenuData.BackgroundColor = FLinearColor(0.25f, 0.f, 0.f, 0.75f);
	MenuData.ButtonGroupData.ButtonGroupOrientation = Orient_Horizontal;
	MenuData.MenuAlignmentData = FAlignmentData(HAlign_Center, VAlign_Center);
	GameOverMenuSlateWidget = SNew(SDefaultMenu).InMenuData(MenuData);
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
	if (PauseMenuWidget.IsValid())
	{
		PauseMenuWidget->SetVisibility(EVisibility::Visible);
	}
	else
	{
		GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(PauseMenuWidget, SWeakWidget).PossiblyNullContent(PauseMenuSlateWidget.ToSharedRef()));
	}
	EnterUIMode();
}

void ASGMainHUD::GameOver()
{
	GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(GameOverMenuWidget, SWeakWidget).PossiblyNullContent(GameOverMenuSlateWidget.ToSharedRef()));
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

void ASGMainHUD::ContinueGame(TSharedPtr<SWeakWidget> CallerWidget)
{
	CallerWidget->SetVisibility(EVisibility::Collapsed);
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

FButtonData ASGMainHUD::CreateMenuButtonData(const FText& ButtonText, const FOnClicked& OnClicked)
{
	FButtonData Button = FButtonData();
	Button.TextData.Text = ButtonText;
	Button.TextData.TextStyle = FStyleSetData::Get().GetWidgetStyle<FTextBlockStyle>(StyleNames::MenuButtonText());
	Button.ButtonStyle = FStyleSetData::Get().GetWidgetStyle<FButtonStyle>(StyleNames::MenuButton());
	Button.OnClicked = OnClicked;

	return Button;
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
