#include "../../../Public/Gear/Weapons/SGGrenadeLauncher.h"
#include "NiagaraFunctionLibrary.h"
#include "../../../Public/Gear/Weapons/SGExplosiveProjectile.h"
#include "../../../Public/Player/SGPlayerCharacter.h"
#include "Core/SGUpgradeSubsystem.h"
#include "Engine/StaticMeshActor.h"
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
	if (!HasAmmo()) return;
	--Ammo;
	
	if (!ProjectileSpawnPoint || !ProjectileClass) return;
	
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

	EnableOverlapEventsForAllStaticMeshes(GetWorld());

	if (USGUpgradeSubsystem* UpgradeSystem = GetGameInstance()->GetSubsystem<USGUpgradeSubsystem>())
	{
		FName Category = TEXT("Grenade Launcher");
		UpgradeSystem->BindAttribute(this, TEXT("Damage"), TEXT("GunDamage"), Category);
		UpgradeSystem->BindAttribute(this, TEXT("FireRate"), TEXT("GunFireRate"), Category);
	}
	
}

// Private
bool ASGGrenadeLauncher::HasAmmo()
{
	return Ammo > 0;
}

void ASGGrenadeLauncher::SpawnProjectile()
{
	if (!ProjectileClass || !ProjectileSpawnPoint) return;

	FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
	AController* Controller = GetOwner() ? GetOwner()->GetInstigatorController() : nullptr;
	FRotator SpawnRotation = Controller ? Controller->GetControlRotation() : FRotator::ZeroRotator;
	SpawnRotation.Pitch += PitchOffsetDegrees;

	FVector LaunchDirection = SpawnRotation.Vector();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	ASGExplosiveProjectile* Projectile = GetWorld()->SpawnActor<ASGExplosiveProjectile>(
		ProjectileClass,
		SpawnLocation,
		SpawnRotation,
		SpawnParams
	);

	if (Projectile)
	{
		//Projectile->SetDamage(Damage);

		UProjectileMovementComponent* MovementComp = Projectile->GetMovementComponent();
		if (MovementComp)
		{
			MovementComp->Velocity = LaunchDirection * LaunchSpeed;
		}
	}
}

void ASGGrenadeLauncher::EnableOverlapEventsForAllStaticMeshes(UWorld* World)
{
	if (!World) return;

	TArray<AActor*> StaticMeshActors;
	UGameplayStatics::GetAllActorsOfClass(World, AStaticMeshActor::StaticClass(), StaticMeshActors);

	for (AActor* Actor : StaticMeshActors)
	{
		if (AStaticMeshActor* StaticMeshActor = Cast<AStaticMeshActor>(Actor))
		{
			if (UStaticMeshComponent* MeshComponent = StaticMeshActor->GetStaticMeshComponent())
			{
				MeshComponent->SetGenerateOverlapEvents(true);
			}
		}
	}
}