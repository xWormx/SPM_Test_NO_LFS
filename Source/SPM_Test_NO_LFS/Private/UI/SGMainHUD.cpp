#include "UI/SGMainHUD.h"

#include "jola6902_GunsComponent.h"
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

static bool HasFirstQuestStarted = false;

void ASGMainHUD::BeginPlay()
{
	Super::BeginPlay();
	FStyleSetData::Initialize();
	HasFirstQuestStarted = false;

	if (!MainHUDWidget.IsValid())
	{
		MainHUDWidget = CreateAndAddToViewPort<USGMainHUDWidget>(MainHUDWidgetClass);
		MainHUDWidget->PauseAndHide();
	}
	
	GetWorld()->GetSubsystem<USGObjectiveHandlerSubSystem>()->OnObjectiveStarted.AddDynamic(this, &ASGMainHUD::StartDifficultyBar);

	/*FButtonData ButtonData = FButtonData();
	ButtonData.ButtonText = FText::FromString("Start Game");
	ButtonData.OnClicked.BindLambda([this]{EMMA_LOG(Warning, TEXT("❤️Start Game Button Clicked!")); EnterPlayState(); return FReply::Handled(); });
	
	DefaultButtonWidgetSlate = SNew(SDefaultButtonWidget).InButtonData(ButtonData);
	GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(DefaultButtonWidget, SWeakWidget).PossiblyNullContent(DefaultButtonWidgetSlate.ToSharedRef()));*/
	/*FButtonData ButtonDataStartGame = FButtonData();
	ButtonDataStartGame.TextData.Text = FText::FromString("Start Game");
	ButtonDataStartGame.TextData.TextStyle = FStyleSetData::Get().GetWidgetStyle<FTextBlockStyle>(StyleNames::MenuButtonText());
	ButtonDataStartGame.ButtonStyle = FStyleSetData::Get().GetWidgetStyle<FButtonStyle>(StyleNames::MenuButton());
	ButtonDataStartGame.OnClicked.BindLambda([this]
	{
		EMMA_LOG(Warning, TEXT("❤️Start Game Button Clicked!"));		
		GEngine->GameViewport->RemoveViewportWidgetContent(DefaultMenuWidget.ToSharedRef());
		EnterPlayState();
		return FReply::Handled();
	});

	FButtonData ButtonDataQuitGame = FButtonData();
	ButtonDataQuitGame.TextData.Text = FText::FromString("Quit Game");
	ButtonDataQuitGame.TextData.TextStyle = FStyleSetData::Get().GetWidgetStyle<FTextBlockStyle>(StyleNames::MenuButtonText());
	ButtonDataQuitGame.ButtonStyle = FStyleSetData::Get().GetWidgetStyle<FButtonStyle>(StyleNames::MenuButton());
	ButtonDataQuitGame.OnClicked.BindLambda([this]
	{
		EMMA_LOG(Warning, TEXT("❤️Quit Game Button Clicked!"));
		if (GEngine)
		{
			GEngine->GameViewport->RemoveAllViewportWidgets();
			PlayerOwner.Get()->ConsoleCommand("quit");
		}
		return FReply::Handled();
	});

	FTextData TextData = FTextData();
	TextData.Text = FText::FromString("Main Menu");
	TextData.TextStyle = FStyleSetData::Get().GetWidgetStyle<FTextBlockStyle>(StyleNames::MenuHeaderText());

	FMenuData MenuData;
	MenuData.TextData = TextData;
	MenuData.ButtonDataArray = { ButtonDataStartGame, ButtonDataQuitGame };
	MenuData.BackgroundColor = FColor::Black;
	MenuData.MenuButtonsOrientation = Orient_Vertical;
	MenuData.MenuAlignmentData = FAlignmentData(HAlign_Center, VAlign_Center);
	DefaultSlateMenuWidget = SNew(SDefaultMenu).InMenuData(MenuData);
	GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(DefaultMenuWidget, SWeakWidget).PossiblyNullContent(DefaultSlateMenuWidget.ToSharedRef()));*/
	InitStartMenu();
	InitPauseMenu();
	InitGameOverMenu();
	
	EnterUIState();
}

void ASGMainHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	USGObjectiveHandlerSubSystem* ObjectiveHandler = GetWorld()->GetSubsystem<USGObjectiveHandlerSubSystem>();
	ObjectiveHandler->OnObjectiveStarted.RemoveDynamic(this, &ASGMainHUD::EnterPlayState);

	Super::EndPlay(EndPlayReason);
}

//---- INITS

void ASGMainHUD::BindToPlayerComponentEvents(ASGPlayerCharacter* PlayerCharacter)
{
	if (!PlayerCharacter)
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

void ASGMainHUD::InitStartMenu()
{
	FButtonData ButtonDataStartGame = FButtonData();
	ButtonDataStartGame.TextData.Text = FText::FromString("Start Game");
	ButtonDataStartGame.TextData.TextStyle = FStyleSetData::Get().GetWidgetStyle<FTextBlockStyle>(StyleNames::MenuButtonText());
	ButtonDataStartGame.ButtonStyle = FStyleSetData::Get().GetWidgetStyle<FButtonStyle>(StyleNames::MenuButton());
	ButtonDataStartGame.OnClicked.BindLambda([this]
	{
		EMMA_LOG(Warning, TEXT("❤️Start Game Button Clicked!"));		
		GEngine->GameViewport->RemoveViewportWidgetContent(StartMenuWidget.ToSharedRef());
		EnterPlayState();
		return FReply::Handled();
	});
	
	FButtonData ButtonDataQuitGame = FButtonData();
	ButtonDataQuitGame.TextData.Text = FText::FromString("Quit Game");
	ButtonDataQuitGame.TextData.TextStyle = FStyleSetData::Get().GetWidgetStyle<FTextBlockStyle>(StyleNames::MenuButtonText());
	ButtonDataQuitGame.ButtonStyle = FStyleSetData::Get().GetWidgetStyle<FButtonStyle>(StyleNames::MenuButton());
	ButtonDataQuitGame.OnClicked.BindLambda([this]
	{
		EMMA_LOG(Warning, TEXT("❤️Quit Game Button Clicked!"));
		if (GEngine)
		{
			GEngine->GameViewport->RemoveAllViewportWidgets();
			PlayerOwner.Get()->ConsoleCommand("quit");
		}
		return FReply::Handled();
	});

	FTextData TextData = FTextData();
	TextData.Text = FText::FromString("Main Menu");
	TextData.TextStyle = FStyleSetData::Get().GetWidgetStyle<FTextBlockStyle>(StyleNames::MenuHeaderText());

	FMenuData MenuData;
	MenuData.TextData = TextData;
	MenuData.ButtonDataArray = { ButtonDataStartGame, ButtonDataQuitGame };
	MenuData.BackgroundColor = FColor::Black;
	MenuData.MenuButtonsOrientation = Orient_Vertical;
	MenuData.MenuAlignmentData = FAlignmentData(HAlign_Center, VAlign_Center);

	StartMenuSlateWidget = SNew(SDefaultMenu).InMenuData(MenuData);
	GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(StartMenuWidget, SWeakWidget).PossiblyNullContent(StartMenuSlateWidget.ToSharedRef()));
}

void ASGMainHUD::InitPauseMenu()
{
	FButtonData ButtonContinueGame = FButtonData();
	ButtonContinueGame.TextData.Text = FText::FromString("Continue Game");
	ButtonContinueGame.TextData.TextStyle = FStyleSetData::Get().GetWidgetStyle<FTextBlockStyle>(StyleNames::MenuButtonText());
	ButtonContinueGame.ButtonStyle = FStyleSetData::Get().GetWidgetStyle<FButtonStyle>(StyleNames::MenuButton());
	ButtonContinueGame.OnClicked.BindLambda([this]
	{
		GEngine->GameViewport->RemoveViewportWidgetContent(PauseMenuWidget.ToSharedRef());
		EnterPlayState();
		return FReply::Handled();
	});

	FButtonData ButtonRestartGame = FButtonData();
	ButtonRestartGame.TextData.Text = FText::FromString("Restart Game");
	ButtonRestartGame.TextData.TextStyle = FStyleSetData::Get().GetWidgetStyle<FTextBlockStyle>(StyleNames::MenuButtonText());
	ButtonRestartGame.ButtonStyle = FStyleSetData::Get().GetWidgetStyle<FButtonStyle>(StyleNames::MenuButton());
	ButtonRestartGame.OnClicked.BindLambda([this]
	{
		if (GEngine)
		{
			GEngine->GameViewport->RemoveAllViewportWidgets();
			UGameplayStatics::SetGamePaused(GetWorld(), false);
			FString CurrentLevel = GetWorld()->GetMapName();
			CurrentLevel.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
			UGameplayStatics::OpenLevel(this, FName(*CurrentLevel));
		}
		return FReply::Handled();
	});

	FButtonData ButtonSaveGame = FButtonData();
	ButtonSaveGame.TextData.Text = FText::FromString("Save Game");
	ButtonSaveGame.TextData.TextStyle = FStyleSetData::Get().GetWidgetStyle<FTextBlockStyle>(StyleNames::MenuButtonText());
	ButtonSaveGame.ButtonStyle = FStyleSetData::Get().GetWidgetStyle<FButtonStyle>(StyleNames::MenuButton());
	ButtonSaveGame.OnClicked.BindLambda([this]
	{		
		GetGameInstance<USGGameInstance>()->CollectAndSave(false);
		return FReply::Handled();
	});

	FButtonData ButtonReturnToMainMenu = FButtonData();
	ButtonReturnToMainMenu.TextData.Text = FText::FromString("Return to Main Menu");
	ButtonReturnToMainMenu.TextData.TextStyle = FStyleSetData::Get().GetWidgetStyle<FTextBlockStyle>(StyleNames::MenuButtonText());
	ButtonReturnToMainMenu.ButtonStyle = FStyleSetData::Get().GetWidgetStyle<FButtonStyle>(StyleNames::MenuButton());
	ButtonReturnToMainMenu.OnClicked.BindLambda([this]
	{
		if (GEngine)
		{
			GEngine->GameViewport->RemoveAllViewportWidgets();
			UGameplayStatics::SetGamePaused(GetWorld(), false);
			GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(StartMenuWidget, SWeakWidget).PossiblyNullContent(StartMenuSlateWidget.ToSharedRef()));
			EnterUIState();
		}
		return FReply::Handled();
	});

	FMenuData MenuData;
	MenuData.TextData = FTextData();
	MenuData.TextData.Text = FText::FromString("Paused");
	MenuData.TextData.TextStyle = FStyleSetData::Get().GetWidgetStyle<FTextBlockStyle>(StyleNames::MenuHeaderText());
	MenuData.ButtonDataArray = { ButtonContinueGame, ButtonRestartGame, ButtonSaveGame, ButtonReturnToMainMenu };
	MenuData.BackgroundColor = FLinearColor(0.f, 0.f, 0.f, 0.8f);
	MenuData.MenuButtonsOrientation = Orient_Vertical;
	MenuData.MenuAlignmentData = FAlignmentData(HAlign_Center, VAlign_Center);
	PauseMenuSlateWidget = SNew(SDefaultMenu).InMenuData(MenuData);	
}

void ASGMainHUD::InitGameOverMenu()
{
	FButtonData ButtonRestartGame = FButtonData();
	ButtonRestartGame.TextData.Text = FText::FromString("Return To Main Menu");
	ButtonRestartGame.TextData.TextStyle = FStyleSetData::Get().GetWidgetStyle<FTextBlockStyle>(StyleNames::MenuButtonText());
	ButtonRestartGame.ButtonStyle = FStyleSetData::Get().GetWidgetStyle<FButtonStyle>(StyleNames::MenuButton());
	ButtonRestartGame.OnClicked.BindLambda([this]
	{
		if (GEngine)
		{
			GEngine->GameViewport->RemoveAllViewportWidgets();
			UGameplayStatics::SetGamePaused(GetWorld(), false);
			FString CurrentLevel = GetWorld()->GetMapName();
			CurrentLevel.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
			UGameplayStatics::OpenLevel(this, FName(*CurrentLevel));
		}
		return FReply::Handled();
	});
	
	FMenuData MenuData;
	MenuData.TextData = FTextData();
	MenuData.TextData.Text = FText::FromString("Paused");
	MenuData.TextData.TextStyle = FStyleSetData::Get().GetWidgetStyle<FTextBlockStyle>(StyleNames::MenuHeaderText());
	MenuData.ButtonDataArray = { ButtonRestartGame };
	MenuData.BackgroundColor = FLinearColor(0.3f, 0.f, 0.f, 0.8f);
	MenuData.MenuButtonsOrientation = Orient_Horizontal;
	MenuData.MenuAlignmentData = FAlignmentData(HAlign_Center, VAlign_Center);
	GameOverMenuSlateWidget = SNew(SDefaultMenu).InMenuData(MenuData);	
}

//---- UI STATE MANAGEMENT

void ASGMainHUD::EnterUIState()
{	
	MainHUDWidget->PauseAndHide();
	
	APlayerController* Controller = GetOwningPlayerController();
	Controller->SetInputMode(FInputModeUIOnly());
	Controller->bShowMouseCursor = true;
	Controller->SetPause(true);

	MainHUDWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void ASGMainHUD::EnterPlayState()
{
	MainHUDWidget->SetVisibility(ESlateVisibility::Visible);
	MainHUDWidget->PlayAndShow();
	
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
		EnterUIState();
	}
	else if (NewVisibility == ESlateVisibility::Hidden || NewVisibility == ESlateVisibility::Collapsed)
	{
		EnterPlayState();
	}
}

void ASGMainHUD::StartDifficultyBar()
{
	MainHUDWidget->StartDifficultyBar();
	GetWorld()->GetSubsystem<USGObjectiveHandlerSubSystem>()->OnObjectiveStarted.RemoveDynamic(this, &ASGMainHUD::StartDifficultyBar);
}

void ASGMainHUD::PauseGame()
{
	EMMA_LOG(Warning, TEXT("❤️Pause Game Button Clicked!"));
	GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(PauseMenuWidget, SWeakWidget).PossiblyNullContent(PauseMenuSlateWidget.ToSharedRef()));
	EnterUIState();
}
void ASGMainHUD::GameOver()
{
	EMMA_LOG(Warning, TEXT("❤️Game Over!"));
	GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(GameOverMenuWidget, SWeakWidget).PossiblyNullContent(GameOverMenuSlateWidget.ToSharedRef()));
	EnterUIState();
}
//----HELPERS
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
