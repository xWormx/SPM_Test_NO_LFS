// Joel Larsson Wendt | jola6902

#pragma once

class ASGGun;
class USGWeaponsHUD;
class UInputAction;
struct FInputActionInstance;
struct FInputActionValue;

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "jola6902_GunsComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPM_TEST_NO_LFS_API Ujola6902_GunsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	Ujola6902_GunsComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	const TArray<ASGGun*> GetGuns() const;

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
	void CreateGunsHUD();
	void UpdateGunsHUD();

	AActor* Owner;
	USGWeaponsHUD* GunsHUD;
	TArray<ASGGun*> Guns;
	int32 CurrentGunIndex = 0;
	bool bIsFiring = false;
	bool bCanFire = false;
	FTimerHandle CanFireAgainTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	FComponentReference GunsAttachment;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	TArray<TSubclassOf<ASGGun>> GunClasses;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	TSubclassOf<UUserWidget> GunsHUDWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	TMap<UInputAction*, int32> GunIndexKeyBindings;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	UInputAction* MouseWheelScrollInputAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	UInputAction* FireGunInputAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	UInputAction* ReloadGunInputAction;
};
