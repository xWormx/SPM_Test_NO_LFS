// Joel Larsson Wendt || jola6902

#pragma once

class ASGGun;
class USGWeaponsHUD;
class UInputAction;
struct FInputActionInstance;
struct FInputActionValue;

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "jola6902_GunsComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSwitchedGun, int32, GunIndex, ASGGun*, Gun);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnReload, int32, GunIndex, ASGGun*, Gun);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFireGun, int32, GunIndex, ASGGun*, Gun);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCanFireGun, bool, bCanFireGun, int32, GunIndex, ASGGun*, Gun);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPM_TEST_NO_LFS_API Ujola6902_GunsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	Ujola6902_GunsComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	const TArray<ASGGun*> GetGuns() const;

	UFUNCTION(BlueprintCallable)
	const ASGGun* GetCurrentGun() const;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	void FireGun();
	void CanFireAgain();
	void ReloadGun();

	void OnFireButtonPressed(const FInputActionValue& Value);
	void OnFireButtonReleased(const FInputActionValue& Value);
	void OnReloadButtonPressed(const FInputActionValue& Value);
	void OnGunIndexKeyPressed(const FInputActionInstance& Instance);
	void OnMouseWheelScroll(const FInputActionValue& Value);
	
	void ValidateKeyBindings();
	void BindActions();
	void SetUpGuns();
	/*void CreateGunsHUD();
	void UpdateGunsHUD();*/

	AActor* Owner;
	/*
	USGWeaponsHUD* GunsHUD;
	*/
	TArray<ASGGun*> Guns;
	int32 CurrentGunIndex = 0;
	bool bCanFire = false;
	bool bFireButtonHeld = false;
	FTimerHandle CanFireAgainTimer;

	// The sibling component to which the Gun mesh will be attached.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	FComponentReference GunsAttachment;
	// All of your derived BP_Gun classes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	TArray<TSubclassOf<ASGGun>> GunClasses;
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	TSubclassOf<UUserWidget> GunsHUDWidgetClass;*/

	// A map of InputActions that correspond to an index in the Guns array
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	TMap<UInputAction*, int32> GunIndexKeyBindings;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	UInputAction* MouseWheelScrollInputAction;
	// Enhanced Input only sends Started and Completed events if the input action has the proper trigger type (Hold), and it’s correctly set up in the Input Action asset. This is required.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	UInputAction* FireGunInputAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	UInputAction* ReloadGunInputAction;

public:
	FOnSwitchedGun OnSwitchedGun;
	FOnReload OnReload;
	FOnFireGun OnFireGun;
	FOnCanFireGun OnCanFireGun;
};
