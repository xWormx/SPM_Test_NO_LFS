// Joel Larsson Wendt || jola6902

#include "Gear/Weapons/SGGun.h"
#include "Core/SGUpgradeSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"
#include "NiagaraFunctionLibrary.h"
#include "SPM_Test_NO_LFS.h"
#include "Player/SGPlayerCharacter.h"

// Public
ASGGun::ASGGun()
{
 	PrimaryActorTick.bCanEverTick = true;
	//Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	//RootComponent = Root;
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	//Mesh->SetupAttachment(Root);
	SetRootComponent(Mesh);
	ShootParticlesPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ShootParticlesPoint"));
	ShootParticlesPoint->SetupAttachment(RootComponent);
}

void ASGGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/* Avfyrar vapnet och kontrollerar om en träff har inträffat, delar ut skada i sådant fall. Basklassen Gun använder
sig utav funktionen HitScan() för att avgöra om en träff har skett. Ärvande klasser är fria att själva definiera hur
hit detection och avfyrning går till. Exempel: SGGrenadeLauncher */
void ASGGun::Fire()
{
	if (bIsReloading) return;

	if (!HasAmmo())
	{
		if (OutOfAmmoSound && Mesh) UGameplayStatics::SpawnSoundAttached(OutOfAmmoSound, Mesh, TEXT("MuzzleFlashSocket"));
		return;
	}
	
	if (bUsesMagazine)
	{
		--CurrentMagazineAmmo;
	}
	else
	{
		if (!bInfiniteAmmo)
		{
			--Ammo;
		}
	}
	
	if (ShootParticles && ShootParticlesPoint)
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(
			ShootParticles,
			ShootParticlesPoint,
			NAME_None,    
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset,
			true
			);
	} //if (ShootParticles && Mesh) UGameplayStatics::SpawnEmitterAttached(ShootParticles, Mesh, TEXT("MuzzleFlashSocket"));
	
	if (ShootSound && Mesh) UGameplayStatics::SpawnSoundAttached(ShootSound, Mesh, TEXT("MuzzleFlashSocket"));

	if (PlayerRef) PlayerRef->BPFire();
	
	FHitResult HitResult;
	FVector ShotDirection;
	if (HitScan(HitResult, ShotDirection))
	{
		//DrawDebugPoint(GetWorld(), HitResult.Location, 10, FColor::Red, true);
		if (HitParticles)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			HitParticles,
			HitResult.Location,
			FRotator::ZeroRotator
		);
		} //if (HitParticles) UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles, HitResult.Location, ShotDirection.Rotation());
		
		if (HitSound) UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, HitResult.Location);

		AActor* HitActor = HitResult.GetActor();
		if (HitActor != nullptr)
		{
			FPointDamageEvent DamageEvent(Damage, HitResult, ShotDirection, nullptr);
			HitActor->TakeDamage(Damage, DamageEvent, GetOwnerController(), this);
		}
	}
}

// Laddar om vapnet ifall att det är möjligt.
bool ASGGun::Reload()
{
	if (!bUsesMagazine || bIsReloading) return false;

	int32 AmmoNeeded = MagazineSize - CurrentMagazineAmmo;
	if (AmmoNeeded <= 0) return false;
	if (!bInfiniteAmmo && Ammo <= 0) return false;

	bIsReloading = true;

	if (PlayerRef) PlayerRef->BPReload();

	if (ReloadSound && Mesh) 
	{
		UGameplayStatics::SpawnSoundAttached(ReloadSound, Mesh, TEXT("MuzzleFlashSocket"));
	}

	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ASGGun::FinishReloading, ReloadTime, false);
	return true;
}

float ASGGun::GetFireRate() const
{
	return FireRate;
}

void ASGGun::AddAmmo(int32 AmmoAmount)
{
	Ammo += AmmoAmount;
}

// Returnerar hur mycket skott vapnet har kvar, om vapnet ej använder magasin används istället ammo-reserverna direkt.
int32 ASGGun::GetAmmoClip()
{
	if (bUsesMagazine)
	{
		return CurrentMagazineAmmo;
	}
	else
	{
		if (!bInfiniteAmmo)
		{
			return Ammo;
		}
	}

	return -1;
}

int32 ASGGun::GetAmmoStock()
{
	if (bInfiniteAmmo)
	{
		return -1;
	}
	
	return Ammo;
}

FText ASGGun::GetWeaponDisplayName()
{
	return WeaponDisplayName;
}

// Protected
void ASGGun::BeginPlay()
{
	Super::BeginPlay();

	PlayerRef = Cast<ASGPlayerCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));

	//CurrentMagazineAmmo = FMath::Min(MagazineSize, Ammo);

	if (USGUpgradeSubsystem* UpgradeSystem = GetGameInstance()->GetSubsystem<USGUpgradeSubsystem>())
	{
		FName Category = TEXT("All Guns");
		UpgradeSystem->BindAttribute(this, TEXT("Damage"), TEXT("GunDamage"), Category);
		UpgradeSystem->BindAttribute(this, TEXT("FireRate"), TEXT("GunFireRate"), Category);

		/* Alla vapen kommer binda ReloadTime då detta är basklassen, men används endast av AR så övriga vapen påverkas inte.
		 * En vackrare lösning som inte innebär att ha (två) dinglande attribut som uppgraderas utan användning vore nice, men eftersom detta
		 * i nuläget är (och förmodligen kommer vara) enda stället det här sker, så låter jag det vara (just nu). Mvh Emma */
		UpgradeSystem->BindAttribute(this, TEXT("ReloadTime"), TEXT("GunReloadSpeed"), TEXT("Assault Rifle"));
	}

	/**
	 * In some sense, a more optimal solution would be to have the materials collected and managed by the GunsComponent,
	 * since every gun now owns a reference to the PlayerRef weaponmesh. This could be considered being some sort of redundency
	 * and it might cause confusion when running the UpdateAndShowAmmoOnMesh function directly on any Gun instance, since only the active Gun should update it,
	 * but it has no way to check if "am I the active Gun".
	 *
	 * Reasons for this to be in the Gun class instead of the GunsComponent is because it requires less changes in the overall codebase - only additions and no changes in the overall logic/flow.
	 * As well as there is a sense of separation of concerns, where the Gun class is responsible for its "own" materials and ammo count. Even though it is attached to the PlayerRef it is still a representation of the Gun itself.
	 * The WeaponHUD calls the UpdateAndShowAmmoOnMesh every time it updates the ammo count on an active Gun. The sense there is that it could be considered "UI" related.
	 *
	 * 💡Simple and possible solutions if this becomes a issue:
	 * - bool, maybe gameplay tag that gets added/removed to used as a safeguard to check if the Gun is the active one, before updating the ammo materials.
	 *		"Requires" changes in the GunsComponent to check and update this, since we are then changing logic-related things and not just UI.
	 * - Moving the ammo materials to the GunsComponent and having it update the materials for the active Gun.
	 *		Requires access to the player's weapon mesh and storing references to the materials,
	 *		and for the GetCurrentGun function to not be returning a const reference or having GetAmmoClip being declared as const.
	 */
	if (PlayerRef && PlayerRef->WeaponMesh)
	{
		/*
		 * Fully possible to just directly set the material on the given index directly (left 1 and right 2).
		 * Looping through them to just make it a tiny bit more robust if some change would happen to the materials in the future.
		 */
		for (int i = 0; i < PlayerRef->WeaponMesh->GetMaterials().Num(); ++i)
		{
			UMaterialInterface* Material = PlayerRef->WeaponMesh->GetMaterial(i);
			FString MaterialName = Material ? Material->GetName() : FString();
			if (!MaterialName.Contains("AmmoCount"))
			{
				continue;
			}
			if (MaterialName.Contains("Left"))
			{
				LeftAmmoCountMaterial = PlayerRef->WeaponMesh->CreateAndSetMaterialInstanceDynamicFromMaterial(i, Material);
			}
			else if (MaterialName.Contains("Right"))
			{
				RightAmmoCountMaterial = PlayerRef->WeaponMesh->CreateAndSetMaterialInstanceDynamicFromMaterial(i, Material);
			}
		}
	}
}

AController* ASGGun::GetOwnerController() const
{
	APawn* MyOwner = Cast<APawn>(GetOwner());
	if (MyOwner == nullptr) return nullptr;
	return MyOwner->GetController();
}

bool ASGGun::HasAmmo()
{
	if (bUsesMagazine)
	{
		return CurrentMagazineAmmo > 0;
	}
	else
	{
		return bInfiniteAmmo || Ammo > 0;
	}
}

// Private
// Kontrollerar om en träff har skett genom att skjuta X antal RayCasts i en kon av inaccuracy (ställbar i editorn).
bool ASGGun::HitScan(FHitResult& OutHitResult, FVector& OutShotDirection)
{
	AController* OwnerController = GetOwnerController();
    if (OwnerController == nullptr) return false;

    FVector Location;
    FRotator Rotation;
    OwnerController->GetPlayerViewPoint(Location, Rotation);
    
    bool bHitSomething = false;
    for (int i = 0; i < NumberOfPellets; ++i)
    {
        float RandomPitch = FMath::FRandRange(-PelletSpreadAngle, PelletSpreadAngle);
        float RandomYaw = FMath::FRandRange(-PelletSpreadAngle, PelletSpreadAngle);
        float RandomRoll = FMath::FRandRange(-PelletSpreadAngle, PelletSpreadAngle);
        
        FRotator PelletRotation = Rotation + FRotator(RandomPitch, RandomYaw, RandomRoll);
        FVector PelletDirection = PelletRotation.Vector();
        FVector End = Location + PelletDirection * MaxRange;
        
        FCollisionQueryParams Params;
        Params.AddIgnoredActor(this);
        Params.AddIgnoredActor(GetOwner());
        
        if (GetWorld()->LineTraceSingleByChannel(OutHitResult, Location, End, ECC_GameTraceChannel2, Params))
        {
            bHitSomething = true;
            
            AActor* HitActor = OutHitResult.GetActor();
            if (HitActor != nullptr)
            {
                FVector ShotDirection = PelletDirection;
                FPointDamageEvent DamageEvent(Damage, OutHitResult, ShotDirection, nullptr);
                HitActor->TakeDamage(Damage, DamageEvent, GetOwnerController(), this);
            }
            
            if (HitParticles)
            {
                UNiagaraFunctionLibrary::SpawnSystemAtLocation(
                GetWorld(),
                HitParticles,
                OutHitResult.Location,
                FRotator::ZeroRotator
            );
            } //UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles, OutHitResult.Location, PelletDirection.Rotation());

            if (HitSound)
            {
                UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, OutHitResult.Location);
            }
        }

        //DrawDebugLine(GetWorld(), Location, End, FColor::Green, false, 1.f, 0, 1.f);
        //DrawDebugPoint(GetWorld(), OutHitResult.Location, 10, FColor::Red, false, 1.f);
    }

    return bHitSomething;
}

// Fördröjd förlängning av Reload() funktionen.
void ASGGun::FinishReloading()
{
	int32 AmmoNeeded = MagazineSize - CurrentMagazineAmmo;

	if (bInfiniteAmmo)
	{
		CurrentMagazineAmmo += AmmoNeeded;
	}
	else
	{
		int32 AmmoToReload = FMath::Min(AmmoNeeded, Ammo);
		CurrentMagazineAmmo += AmmoToReload;
		Ammo -= AmmoToReload;
	}

	bIsReloading = false;
}

void ASGGun::UpdateAndShowAmmoOnMesh()
{
	if (!LeftAmmoCountMaterial || !RightAmmoCountMaterial)
	{
		EMMA_LOG(Error, TEXT("🔫ASGGun::UpdateAndShowAmmoOnMesh() | Left or Right AmmoCount material is not set!"));
		return;
	}

	float TensFloat = static_cast<float>(GetAmmoClip() / 10);
	float OnesFloat = static_cast<float>(GetAmmoClip() % 10);

	LeftAmmoCountMaterial->SetScalarParameterValue(TEXT("AmmoCount"), TensFloat / 10.f);
	RightAmmoCountMaterial->SetScalarParameterValue(TEXT("AmmoCount"), OnesFloat / 10.f);
}
