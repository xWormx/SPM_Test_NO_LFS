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
	float GetFireRate() const;

protected:
	virtual void BeginPlay() override;
	AController* GetOwnerController() const;

	// VisibleAnywhere
	UPROPERTY(VisibleAnywhere, Category="design")
	USceneComponent* Root;
	UPROPERTY(VisibleAnywhere, Category="design")
	USkeletalMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, Category="design")
	USceneComponent* ShootParticlesPoint;
	// EditAnywhere+BP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	UNiagaraSystem* ShootParticles;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	USoundBase* ShootSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	UNiagaraSystem* HitParticles;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	USoundBase* HitSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	float MaxRange = 5000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	float Damage = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	float FireRate = 0.1f; // 0.1 = 10 skott per sekund
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	int NumberOfPellets = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	float PelletSpreadAngle = 3.f;

private:
	virtual bool HitScan(FHitResult& OutHitResult, FVector& OutShotDirection);
};
