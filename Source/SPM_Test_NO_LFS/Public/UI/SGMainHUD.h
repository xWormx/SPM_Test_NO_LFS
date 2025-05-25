#pragma once

#include "CoreMinimal.h"
#include "jola6902_GunsComponent.h"
#include "GameFramework/HUD.h"
#include "SGMainHUD.generated.h"

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

	template<typename T = UUserWidget>
	T* CreateAndAddToViewPort(const TSubclassOf<T>& WidgetClass, bool Add = true);

	void BindToGrappleEvents(ASGGrapplingHook* GrapplingHook);
	void BindWeaponEvents(Ujola6902_GunsComponent* GunsComponent);
	void BindToAmmoEvents(USGCounterComponentAmmo* AmmoComponent);

	void PauseAndHide();

	UFUNCTION(BlueprintCallable, Category = "UFunction - HUD") // Tillfälligt används av BP-scriptet i PlayerController
	void PlayAndShow();
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
	
public:

//----GRAPPLING
	UFUNCTION()
	void OnBeginGrappleCooldown(FTimerHandle& GrappleTimerHandle);

	UFUNCTION()
	void OnFireGrapple();

};
