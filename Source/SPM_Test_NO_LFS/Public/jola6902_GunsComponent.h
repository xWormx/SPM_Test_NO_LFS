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

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	void FireGun();
	void ReloadGun();
	const ASGGun* GetGunRef() const;
	void SetCurrentGunIndex(const int32 NewIndex);
	int32 GetCurrentGunIndex();
	const TArray<ASGGun*>& GetGuns() const;
	void OnKeyPressed(const FInputActionInstance& Instance);
	void OnMouseWheelScroll(const FInputActionValue& Value);

private:
	void ValidateKeyBindings();
	void BindActions();
	void SetUpGuns();
	void CreateGunsHUD();
	void UpdateGunsHUD();

	AActor* Owner;
	USGWeaponsHUD* GunsHUD;
	TArray<ASGGun*> Guns;
	int32 CurrentGunIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	FComponentReference GunsAttachment;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	TArray<TSubclassOf<ASGGun>> GunClasses;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	TMap<UInputAction*, int32> KeyBindings;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	UInputAction* MouseWheelInputAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	TSubclassOf<UUserWidget> GunsHUDWidgetClass;
};
