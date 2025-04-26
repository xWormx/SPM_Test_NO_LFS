#pragma once

#include "CoreMinimal.h"
#include "SGGun.h"
#include "SGShotgun.generated.h"

UCLASS()
class SPM_TEST_NO_LFS_API ASGShotgun : public ASGGun
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

private:
	virtual bool HitScan(FHitResult& OutHitResult, FVector& OutShotDirection) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	int NumberOfPellets = 16;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="design", meta=(AllowPrivateAccess="true"))
	float PelletSpreadAngle = 10.f;
};
