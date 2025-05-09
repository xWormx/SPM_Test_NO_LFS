#include "Gear/Weapons/SGGun.h"
#include "Core/SGUpgradeSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"
#include "NiagaraFunctionLibrary.h"

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

void ASGGun::Fire()
{
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

float ASGGun::GetFireRate() const
{
	return FireRate;
}

// Protected
void ASGGun::BeginPlay()
{
	Super::BeginPlay();

	if (USGUpgradeSubsystem* UpgradeSystem = GetGameInstance()->GetSubsystem<USGUpgradeSubsystem>())
	{
		FName Category = TEXT("Assault Rifle");
		UpgradeSystem->BindAttribute(this, TEXT("Damage"), TEXT("GunDamage"), Category);
		UpgradeSystem->BindAttribute(this, TEXT("FireRate"), TEXT("GunFireRate"), Category);
	}
}

AController* ASGGun::GetOwnerController() const
{
	APawn* MyOwner = Cast<APawn>(GetOwner());
	if (MyOwner == nullptr) return nullptr;
	return MyOwner->GetController();
}

// Private
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