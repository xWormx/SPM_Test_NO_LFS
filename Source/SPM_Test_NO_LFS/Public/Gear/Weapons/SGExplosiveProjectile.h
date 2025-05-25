// Joel Larsson Wendt || jola6902

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGExplosiveProjectile.generated.h"

class UNiagaraSystem;

UCLASS(Blueprintable)
class SPM_TEST_NO_LFS_API ASGExplosiveProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ASGExplosiveProjectile();
	virtual void Tick(float DeltaTime) override;
	void SetDamage(float NewDamage);
	class UProjectileMovementComponent* GetMovementComponent();

protected:
	virtual void BeginPlay() override;

private:
	void DoSpecialEffects();
	
	// VisibleAnywhere
	UPROPERTY(VisibleAnywhere, Category="design")
	USceneComponent* Root;
	UPROPERTY(VisibleAnywhere, Category="design")
	USkeletalMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, Category="design")
	class USphereComponent* SphereCollider;
	UPROPERTY(VisibleAnywhere, Category="design")
	UProjectileMovementComponent* ProjectileMovement;
	// EditAnywhere+BP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	float Damage = 1000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	UMaterialInterface* ExplosionDecal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	float ExplosionDecalRange = 200.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	float ExplosionDecalSize = 200.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	float ExplosionDecalLifetime = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	UParticleSystem* ExplodeEffect;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="design",meta=(AllowPrivateAccess="true"))
	UNiagaraSystem* SmokeTrail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	USoundBase* ExplodeSound;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="design",meta=(AllowPrivateAccess="true"))
	TSubclassOf<class UCameraShakeBase> CameraShakeClass;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="design",meta=(AllowPrivateAccess="true"))
	float ExplosionRadius = 250.f;

	UFUNCTION() // Callback function
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
