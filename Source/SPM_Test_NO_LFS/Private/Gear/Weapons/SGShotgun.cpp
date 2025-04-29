#include "Gear/Weapons/SGShotgun.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"

// Protected
void ASGShotgun::BeginPlay()
{
	Super::BeginPlay();
}

// Private
bool ASGShotgun::HitScan(FHitResult& OutHitResult, FVector& OutShotDirection)
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
                UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles, OutHitResult.Location, PelletDirection.Rotation());
            }

            if (HitSound)
            {
                UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, OutHitResult.Location);
            }
        }

        DrawDebugLine(GetWorld(), Location, End, FColor::Green, false, 1.f, 0, 1.f);
        DrawDebugPoint(GetWorld(), OutHitResult.Location, 10, FColor::Red, false, 1.f);
    }

    return bHitSomething;
}
