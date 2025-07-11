// Joel Larsson Wendt || jola6902

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGGun.generated.h"

class UNiagaraSystem;

UCLASS()
class SPM_TEST_NO_LFS_API ASGGun : public AActor // Basklass för skjutvapen (används ihop med jola6902_GunsComponent)
{
	GENERATED_BODY()
	
public:	
	ASGGun();
	virtual void Tick(float DeltaTime) override;
	
	virtual void Fire();
	virtual bool Reload();
	
	float GetFireRate() const;
	void AddAmmo(int32 AmmoAmount);

	UFUNCTION(BlueprintCallable)
	int32 GetAmmoClip();
	
	int32 GetAmmoStock();
	FText GetWeaponDisplayName();

protected:
	virtual void BeginPlay() override;
	AController* GetOwnerController() const;

	class ASGPlayerCharacter* PlayerRef;

	UFUNCTION(BlueprintCallable)
	virtual bool HasAmmo();

	FTimerHandle ReloadTimerHandle;

	UPROPERTY(BlueprintReadOnly)
	bool bIsReloading = false;

	// VisibleAnywhere
	UPROPERTY(VisibleAnywhere, Category="design")
	USceneComponent* Root;
	UPROPERTY(VisibleAnywhere, Category="design")
	USkeletalMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, Category="design")
	USceneComponent* ShootParticlesPoint;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="design", meta=(AllowPrivateAccess="true"))
	int32 Ammo = 1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="design", meta=(AllowPrivateAccess="true"))
	int32 CurrentMagazineAmmo = 30;
	// EditAnywhere
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	FText WeaponDisplayName = FText::FromString("AssaultRifle");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	UNiagaraSystem* ShootParticles;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	UNiagaraSystem* HitParticles;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	USoundBase* ShootSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	USoundBase* HitSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	USoundBase* ReloadSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	USoundBase* OutOfAmmoSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	float MaxRange = 5000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	float Damage = 100.f;
	// How often the Gun can shoot (0.1 = ten shots per second, 2.0 = one shot every other second)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	float FireRate = 0.1f;
	// Number of bullets fired at a time in a cone of accuracy (1 pellet with low PelletSpreadAngle = AssaultRifle, Many pellets with high PelletSpreadAngle = Shotgun)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	int NumberOfPellets = 1;
	// Decides the amount of inaccuracy, or rather the size of the cone that shots are randomly fired in
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	float PelletSpreadAngle = 3.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	int32 MagazineSize = 30;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	float ReloadTime = 2.0f;
	// If false the weapon will never need to be reloaded
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	bool bUsesMagazine = true;
	// If true the weapon has infinite ammo in stock, can still have a limited clip size (See: bUsesMagazine)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	bool bInfiniteAmmo = true;

private:
	virtual bool HitScan(FHitResult& OutHitResult, FVector& OutShotDirection);
	void FinishReloading();

public:
	/**
	 * Changes the ammo count displayed on the player weapon mesh.
	 *
	 * @warning ⚠️Before calling this function, verify this is the active gun in the GunsComponent.
	 * Otherwise, an inactive gun may override what is displayed.️
	 *
	 * Example usage:
	 * @code
	 * if (GunsComponent->GetCurrentGun() == this)
	 * {
	 *     UpdateAmmoMaterials();
	 * }
	 * @endcode
	 */
	UFUNCTION(BlueprintCallable, Category="UFunction - Gun", meta=(ToolTip ="Before calling this function, make sure to check if the gun is the active gun in the GunsComponent"))
	void UpdateAndShowAmmoOnMesh();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UProperty - Gun", meta=(AllowPrivateAccess="true"))
	UMaterialInstanceDynamic* LeftAmmoCountMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UProperty - Gun", meta=(AllowPrivateAccess="true"))
	UMaterialInstanceDynamic* RightAmmoCountMaterial;

};
