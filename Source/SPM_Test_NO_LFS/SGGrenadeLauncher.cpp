#include "SGGrenadeLauncher.h"

#include "SGExplosiveProjectile.h"
#include "Kismet/GameplayStatics.h"

// Public
ASGGrenadeLauncher::ASGGrenadeLauncher()
{
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(RootComponent);
}

void ASGGrenadeLauncher::Fire()
{
	if (!ProjectileSpawnPoint || !ProjectileClass) return;
	
	if (ShootParticles && Mesh) UGameplayStatics::SpawnEmitterAttached(ShootParticles, Mesh, TEXT("MuzzleFlashSocket"));
	if (ShootSound && Mesh) UGameplayStatics::SpawnSoundAttached(ShootSound, Mesh, TEXT("MuzzleFlashSocket"));

	FVector Location = ProjectileSpawnPoint->GetComponentLocation();
	FRotator Rotation = ProjectileSpawnPoint->GetComponentRotation();
	
	DrawDebugSphere(
		GetWorld(),
		ProjectileSpawnPoint->GetComponentLocation(),
		20,
		12,
		FColor::Green,
		false,
		3.f
		); 

	ASGExplosiveProjectile* Projectile = GetWorld()->SpawnActor<ASGExplosiveProjectile>(ProjectileClass, Location, Rotation);
	Projectile->SetOwner(this);
}
