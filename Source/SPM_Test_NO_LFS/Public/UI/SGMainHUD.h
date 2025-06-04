#pragma once

#include "CoreMinimal.h"
#include "Gear/Weapons/jola6902_GunsComponent.h"
#include "GameFramework/HUD.h"
#include "SGMainHUD.generated.h"

class ASGObjectiveFinalSweep;
class USGEndGameInteractWidget;
class USGProgressTrackerWidget;
class USGStatTrackerWidget;
class USGDifficultyBarWidget;
class USGCounterComponentAmmo;
class USGWeaponsHUD;
class USGTerminalWidget;
class USGObjectiveToolTipWidget;
class ASGGrapplingHook;
class USGHUDGrapple;
class ASGGun;

UCLASS()
class SPM_TEST_NO_LFS_API ASGMainHUD : public AHUD
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	template<typename T = UUserWidget>
	T* CreateAndAddToViewPort(const TSubclassOf<T>& WidgetClass, bool Add = true);

	void BindToGrappleEvents(ASGGrapplingHook* GrapplingHook);
	void BindWeaponEvents(Ujola6902_GunsComponent* GunsComponent);
	void BindToAmmoEvents(USGCounterComponentAmmo* AmmoComponent);
	void BindToEndGameInteractEvents(ASGObjectiveFinalSweep* FinalSweepObjective);

	void PauseAndHide();

	UFUNCTION(BlueprintCallable, Category = "UFunction - HUD") // Tillfälligt används av BP-scriptet i PlayerController
	void PlayAndShow();
	UFUNCTION() // Tillfälligt används för att kolla när terminaln är öppen eller stängd
	void OnTerminalVisibilityChanged(ESlateVisibility NewVisibility);

	UFUNCTION() // Tillfälligt för att kolla första gången spelaren startar ett mission
	void StartDifficultyBar();

	UFUNCTION() // Tillfälligt för att gömma vissa widgets så de inte överlappar medan nya questet visas och kör sin animation
	void WaitForObjectiveToolTipAnimation();
protected:
//------------GEAR------------
	//----WEAPONS
	UPROPERTY(EditAnywhere, Category ="Uproperty - HUD")
	TSubclassOf<USGWeaponsHUD> WeaponsClass;

	UPROPERTY(BlueprintReadOnly, Category ="Uproperty - HUD", meta=(BindWidget))
	TWeakObjectPtr<USGWeaponsHUD> WeaponsWidget;

	//----GRAPPLING
	UPROPERTY(EditAnywhere, Category ="Uproperty - HUD")
	TSubclassOf<USGHUDGrapple> GrappleCrossHairClass;

	UPROPERTY(BlueprintReadOnly, Category ="Uproperty - HUD", meta=(BindWidget))
	TWeakObjectPtr<USGHUDGrapple> GrappleCrossHairWidget;

//------------OBJECTIVES------------
	//----MISSION INFO
	UPROPERTY(EditAnywhere, Category ="Uproperty - HUD")
	TSubclassOf<USGObjectiveToolTipWidget> ObjectiveClass;

	UPROPERTY(BlueprintReadOnly, Category ="Uproperty - HUD", meta=(BindWidget))
	TWeakObjectPtr<USGObjectiveToolTipWidget> ObjectiveWidget;

	//----TERMINAL
	UPROPERTY(EditAnywhere, Category ="Uproperty - HUD")
	TSubclassOf<USGTerminalWidget> TerminalClass;

	UPROPERTY(BlueprintReadOnly, Category ="Uproperty - HUD", meta=(BindWidget))
	TWeakObjectPtr<USGTerminalWidget> TerminalWidget;
	
	//----ENDGAME INTERACT
	UPROPERTY(EditAnywhere, Category ="Uproperty - HUD")
	TSubclassOf<USGEndGameInteractWidget> EndGameInteractClass;

	UPROPERTY(BlueprintReadOnly, Category ="Uproperty - HUD", meta=(BindWidget))
	TWeakObjectPtr<USGEndGameInteractWidget> EndGameInteractWidget;
	
//------------HUD------------
	//----CONTAINER
	UPROPERTY(EditAnywhere, Category = "Uproperty - HUD")
	TSubclassOf<UUserWidget> HUDClass;

	UPROPERTY(BlueprintReadOnly, Category ="Uproperty - HUD", meta=(BindWidget))
	TWeakObjectPtr<UUserWidget> HUDWidget;
	
	//----DIFFICULTY BAR
	UPROPERTY(EditAnywhere, Category = "Uproperty - HUD")
	TSubclassOf<USGDifficultyBarWidget> DifficultyClass;

	UPROPERTY(BlueprintReadOnly, Category ="Uproperty - HUD", meta=(BindWidget))
	TWeakObjectPtr<USGDifficultyBarWidget> DifficultyWidget;

//------------PLAYER STATS
	//----HEALTH

/*
	UPROPERTY(EditAnywhere, Category = "Uproperty - HUD")
	TSubclassOf<USGProgressTrackerWidget> ProgressTrackerClass;

	UPROPERTY(BlueprintReadOnly, Category ="Uproperty - HUD", meta=(BindWidget))
	TWeakObjectPtr<USGProgressTrackerWidget> HealthTrackerWidget;

	UPROPERTY(BlueprintReadOnly, Category ="Uproperty - HUD", meta=(BindWidget))
	TWeakObjectPtr<USGProgressTrackerWidget> ShieldTrackerWidget;

	//----ORBS
	UPROPERTY(EditAnywhere, Category = "Uproperty - HUD")
	TSubclassOf<USGStatTrackerWidget> StatTrackerClass;

	UPROPERTY(BlueprintReadOnly, Category ="Uproperty - HUD", meta=(BindWidget))
	TWeakObjectPtr<USGStatTrackerWidget> OrbsTrackerWidget;
*/
public:

//----GRAPPLING
	UFUNCTION()
	void OnBeginGrappleCooldown(FTimerHandle& GrappleTimerHandle);

	UFUNCTION()
	void OnFireGrapple();

};
