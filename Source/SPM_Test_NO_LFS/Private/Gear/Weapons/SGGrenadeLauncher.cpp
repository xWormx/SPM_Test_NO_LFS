#include "../../../Public/Gear/Weapons/SGGrenadeLauncher.h"
#include "../../../Public/Gear/Weapons/SGExplosiveProjectile.h"
#include "../../../Public/Player/SGPlayerCharacter.h"
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
	if (MyOwner) PlayerMesh = MyOwner->GetMesh();
}

// Private
void ASGGrenadeLauncher::SpawnProjectile()
{
	FVector Location = ProjectileSpawnPoint->GetComponentLocation();
	FRotator Rotation = ProjectileSpawnPoint->GetComponentRotation();
	
	ASGExplosiveProjectile* Projectile = GetWorld()->SpawnActor<ASGExplosiveProjectile>(ProjectileClass, Location, Rotation);
	Projectile->SetOwner(this);
	
	FRotator AdjustedRotation = ProjectileSpawnPoint->GetComponentRotation() + FRotator(0, -90.f, 0);
	FVector LaunchDirection = AdjustedRotation.Vector();
	
	USkeletalMeshComponent* ProjectileMesh = Projectile->FindComponentByClass<USkeletalMeshComponent>();
	if (ProjectileMesh && ProjectileMesh->IsSimulatingPhysics() && PlayerMesh)
	{
		//ProjectileMesh->IgnoreComponentWhenMoving(Cast<UPrimitiveComponent>(PlayerMesh), true);
		//ProjectileMesh->IgnoreComponentWhenMoving(Cast<UPrimitiveComponent>(Mesh), true);
		//ProjectileMesh->IgnoreActorWhenMoving(GetOwner(), true);
		//ProjectileMesh->IgnoreActorWhenMoving(this, true);
		ProjectileMesh->MoveIgnoreActors.Add(GetOwner());
		ProjectileMesh->MoveIgnoreActors.Add(this);

		FVector Impulse = LaunchDirection * LaunchSpeed;
		ProjectileMesh->AddImpulse(Impulse, NAME_None, true);
	}
}