#include "../../../Public/Gear/Weapons/SGGrenadeLauncher.h"

#include "HeadMountedDisplayTypes.h"
#include "../../../Public/Gear/Weapons/SGExplosiveProjectile.h"
#include "../../../Public/Player/SGPlayerCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"
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

	SpawnProjectile();
}

// Protected
void ASGGrenadeLauncher::BeginPlay()
{
	Super::BeginPlay();
	ASGPlayerCharacter* MyOwner = Cast<ASGPlayerCharacter>(GetOwner());
	if (MyOwner)
	{
		PlayerMesh = MyOwner->GetMesh();
		PlayerController = MyOwner->GetController();
	}
}

// Private
void ASGGrenadeLauncher::SpawnProjectile()
{
	if (!ProjectileClass || !ProjectileSpawnPoint) return;

	FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
	FRotator ViewRotation = ProjectileSpawnPoint->GetComponentRotation(); // Fallback default
	
	if (PlayerController)
	{
		ViewRotation = PlayerController->GetControlRotation();
	}
	
	ViewRotation.Yaw += 90.f;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	ASGExplosiveProjectile* Projectile = GetWorld()->SpawnActor<ASGExplosiveProjectile>(
		ProjectileClass,
		SpawnLocation,
		ViewRotation,
		SpawnParams
	);

	if (Projectile)
	{
		FVector LaunchDirection = ViewRotation.Vector();
		Projectile->GetMovementComponent()->Velocity = LaunchDirection * LaunchSpeed;
		Projectile->SetDamage(Damage);
	}
}