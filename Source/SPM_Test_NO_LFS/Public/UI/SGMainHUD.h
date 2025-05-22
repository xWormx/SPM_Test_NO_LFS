#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SGMainHUD.generated.h"

class USGWeaponsHUD;
class USGTerminalWidget;
class USGObjectiveToolTipWidget;
class ASGGrapplingHook;
class USGHUDGrapple;

UCLASS()
class SPM_TEST_NO_LFS_API ASGMainHUD : public AHUD
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;

	template<typename T = UUserWidget>
	T* CreateAndAddToViewPort(const TSubclassOf<T>& WidgetClass, bool Add = true);

	void BindToGrappleEvents(ASGGrapplingHook* GrapplingHook);

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
	UPROPERTY(EditDefaultsOnly, Category = "Uproperty - HUD")
	TSubclassOf<UUserWidget> HUDClass;

	UPROPERTY(BlueprintReadOnly, Category ="Uproperty - HUD", meta=(BindWidget))
	TWeakObjectPtr<UUserWidget> HUDWidget;

public:
	UFUNCTION()
	void OnBeginGrappleCooldown(FTimerHandle& GrappleTimerHandle);

	UFUNCTION()
	void OnFireGrapple();

	UFUNCTION()
	void OnCanGrapple(bool bCanGrapple);
};
