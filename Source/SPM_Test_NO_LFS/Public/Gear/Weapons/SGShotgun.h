// Joel Larsson Wendt || jola6902

#pragma once

#include "CoreMinimal.h"
#include "SGGun.h"
#include "SGShotgun.generated.h"

/* Denna klass är egentligen onödig för min del då jag ändrade basklassen till att hantera antal kulor per skott och
RayCasts via en kon av inaccuracy. Så Shotgun hade bara kunnat vara ett BP-child av Gun med andra inställningar.
Men eftersom en gruppmedlem redan använde denna klass i BeginPlay() så lät jag den ligga kvar. */
UCLASS()
class SPM_TEST_NO_LFS_API ASGShotgun : public ASGGun
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
};
