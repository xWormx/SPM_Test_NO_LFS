#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGGun.generated.h"

class UNiagaraSystem;

UCLASS()
class SPM_TEST_NO_LFS_API ASGGun : public AActor
{
	GENERATED_BODY()
	
public:	
	ASGGun();
	virtual void Tick(float DeltaTime) override;
	virtual void Fire();
	virtual void Reload();
	float GetFireRate() const;
	void AddAmmo(int32 AmmoAmount);
	int32 GetAmmoClip();
	int32 GetAmmoStock();
	FText GetWeaponDisplayName();

protected:
	virtual void BeginPlay() override;
	AController* GetOwnerController() const;
	virtual bool HasAmmo();

	FTimerHandle ReloadTimerHandle;
	bool bIsReloading = false;

	// VisibleAnywhere
	UPROPERTY(VisibleAnywhere, Category="design")
	USceneComponent* Root;
	UPROPERTY(VisibleAnywhere, Category="design")
	USkeletalMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, Category="design")
	USceneComponent* ShootParticlesPoint;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="design", meta=(AllowPrivateAccess="true"))
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
	float MaxRange = 5000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	float Damage = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	float FireRate = 0.1f; // Hur ofta vapnet kan skjuta (0.1 = tio skott per sekund, 2.0 = ett skott varannan sekund)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	int NumberOfPellets = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	float PelletSpreadAngle = 3.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	int32 MagazineSize = 30;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	float ReloadTime = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	bool bUsesMagazine = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	bool bInfiniteAmmo = true;

private:
	virtual bool HitScan(FHitResult& OutHitResult, FVector& OutShotDirection);
	void FinishReloading();
};