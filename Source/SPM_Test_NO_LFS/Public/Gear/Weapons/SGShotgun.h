// Joel Larsson Wendt || jola6902

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
};
