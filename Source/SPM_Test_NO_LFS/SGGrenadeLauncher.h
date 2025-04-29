#pragma once

#include "CoreMinimal.h"
#include "SGGun.h"
#include "SGGrenadeLauncher.generated.h"

UCLASS()
class SPM_TEST_NO_LFS_API ASGGrenadeLauncher : public ASGGun
{
	GENERATED_BODY()

public:
	ASGGrenadeLauncher();
	virtual void Fire();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	TSubclassOf<class ASGExplosiveProjectile> ProjectileClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	USceneComponent* ProjectileSpawnPoint;
};
