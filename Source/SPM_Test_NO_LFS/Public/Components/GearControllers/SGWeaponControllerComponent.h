#pragma once

#include "CoreMinimal.h"
#include "SGGearControllerComponent.h"
#include "SGWeaponControllerComponent.generated.h"


class UInputAction;
struct FInputActionValue;
class ASGGun;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SPM_TEST_NO_LFS_API USGWeaponControllerComponent : public USGGearControllerComponent
{
	GENERATED_BODY()

public:
	USGWeaponControllerComponent();

protected:
	virtual void InitGear() override;
	virtual void SetUpInputs() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void FireGun();
	void SetCurrentGunIndex(int8 NewIndex);
	const TArray<ASGGun*>& GetGuns() const;

protected:
	void OnFireButtonPressed(const FInputActionValue& Value);
	void OnFireButtonReleased(const FInputActionValue& Value);
	void HandleFiring();
	void CanFireAgain();
	void OnSwapWeaponKey1Pressed(const FInputActionValue& Value);
	void OnSwapWeaponKey2Pressed(const FInputActionValue& Value);
	void OnSwapWeaponKey3Pressed(const FInputActionValue& Value);
	void OnSwapWeaponMouseWheel(const FInputActionValue& Value);
	
private:

	UPROPERTY(EditAnywhere, Category = "UProperty - Input", meta = (AllowPrivateAccess = "true"))
	FVector WeaponMeshLocationOffset = FVector(50.f,30.f,-20.f);

	UPROPERTY(EditAnywhere, Category = "UProperty - Input", meta = (AllowPrivateAccess = "true"))
	FRotator WeaponMeshRotationOffset = FRotator(0.f,-80.f,0.f); //(0.f,0.f,-80.f);

	UPROPERTY()
	class UInputTriggerPulse* PulseTrigger;
	
	FTimerHandle CanFireAgainTimer;
	bool bIsFiring = false;
	bool bCanFire = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UProperty - Input", meta=(AllowPrivateAccess="true"))
	TArray<TSubclassOf<ASGGun>> GunClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UProperty - Input", meta=(AllowPrivateAccess="true"))
	TArray<ASGGun*> Guns;

	int8 CurrentGunIndex = 0;

	UPROPERTY(VisibleAnywhere, Category="UProperty - Input")
	USkeletalMeshComponent* WeaponMesh;
	//MESH: Script/Engine.SkeletalMesh'/Game/Weapon/NewTest/Weapon_FullRange.Weapon_FullRange'
	//MATERIAL: MI_Template_BaseGray_02_Metal

	UPROPERTY(EditDefaultsOnly, Category = "UProperty - Input")
	UInputAction* FireGunInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "UProperty - Input")
	UInputAction* StopFireGunInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "UProperty - Input")
	UInputAction* SwapWeapon1InputAction;

	UPROPERTY(EditDefaultsOnly, Category = "UProperty - Input")
	UInputAction* SwapWeapon2InputAction;

	UPROPERTY(EditDefaultsOnly, Category = "UProperty - Input")
	UInputAction* SwapWeapon3InputAction;

	UPROPERTY(EditDefaultsOnly, Category = "UProperty - Input")
	UInputAction* SwapWeaponMouseWheelInputAction;

};
