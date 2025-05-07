#include "Gear/Weapons/SGShotgun.h"

#include "Core/SGUpgradeSubsystem.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"

// Protected
void ASGShotgun::BeginPlay()
{
	Super::BeginPlay();

    if (USGUpgradeSubsystem* UpgradeSystem = GetGameInstance()->GetSubsystem<USGUpgradeSubsystem>())
    {
        FName Category = TEXT("Shotgun");
        UpgradeSystem->BindAttribute(this, TEXT("Damage"), TEXT("GunDamage"), Category);
        UpgradeSystem->BindAttribute(this, TEXT("FireRate"), TEXT("GunFireRate"), Category);
    }
}
