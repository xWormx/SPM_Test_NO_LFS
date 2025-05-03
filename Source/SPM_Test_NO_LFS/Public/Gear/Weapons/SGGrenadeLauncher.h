#pragma once

#include "CoreMinimal.h"
#include "SGGun.h"
#include "SGGrenadeLauncher.generated.h"

class ASGExplosiveProjectile;

UCLASS()
class SPM_TEST_NO_LFS_API ASGGrenadeLauncher : public ASGGun
{
	GENERATED_BODY()

public:
	ASGGrenadeLauncher();
	virtual void Fire() override;

protected:
	virtual void BeginPlay() override;

private:
	USkeletalMeshComponent* PlayerMesh;
	AController* PlayerController;

	void SpawnProjectile();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	TSubclassOf<ASGExplosiveProjectile> ProjectileClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	USceneComponent* ProjectileSpawnPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	float LaunchSpeed = 2000.f;
};
