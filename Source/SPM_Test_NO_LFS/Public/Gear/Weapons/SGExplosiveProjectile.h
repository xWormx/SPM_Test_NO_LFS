#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGExplosiveProjectile.generated.h"

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
	float Damage;
	
	void DoSpecialEffects();
	
	// VisibleAnywhere
	UPROPERTY(VisibleAnywhere, Category="design")
	USceneComponent* Root;
	UPROPERTY(VisibleAnywhere, Category="design")
	USkeletalMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, Category="design")
	UProjectileMovementComponent* ProjectileMovement;
	// EditAnywhere+BP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	UParticleSystem* ExplodeEffect;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="design",meta=(AllowPrivateAccess="true"))
	UParticleSystemComponent* SmokeTrail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	USoundBase* ExplodeSound;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="design",meta=(AllowPrivateAccess="true"))
	TSubclassOf<class UCameraShakeBase> CameraShakeClass;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="design",meta=(AllowPrivateAccess="true"))
	float ExplosionRadius = 500.f;

	UFUNCTION() // Callback function
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
