#include "../../../Public/Gear/Weapons/SGGrenadeLauncher.h"
#include "../../../Public/Gear/Weapons/SGExplosiveProjectile.h"
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
	
	FRotator AdjustedRotation = ProjectileSpawnPoint->GetComponentRotation() + FRotator(0, -90.f, 0);
	FVector LaunchDirection = AdjustedRotation.Vector();
	
	USkeletalMeshComponent* ProjectileMesh = Projectile->FindComponentByClass<USkeletalMeshComponent>();
	if (ProjectileMesh && ProjectileMesh->IsSimulatingPhysics())
	{
		ProjectileMesh->IgnoreActorWhenMoving(GetOwner(), true);
		ProjectileMesh->SetPhysicsLinearVelocity(LaunchDirection * LaunchSpeed, false);
	}
}

