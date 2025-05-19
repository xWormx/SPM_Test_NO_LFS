#include "Gear/Weapons/SGShotgun.h"
#include "Core/SGUpgradeSubsystem.h"

void ASGShotgun::BeginPlay()
{
	Super::BeginPlay();

	if (USGUpgradeSubsystem * UpgradeSystem = GetGameInstance()->GetSubsystem<USGUpgradeSubsystem>())
	{
		UpgradeSystem->BindAttribute(this, TEXT("PelletSpreadAngle"), TEXT("GunPelletSpreadAngle"), TEXT("Shotgun"));
	}
}
