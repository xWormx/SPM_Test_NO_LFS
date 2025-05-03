#include "../../../Public/Gear/Weapons/SGExplosiveProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Public
ASGExplosiveProjectile::ASGExplosiveProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionProfileName("BlockAllDynamic");
	Mesh->SetSimulatePhysics(false); // Let ProjectileMovement handle movement
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
}


void ASGExplosiveProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASGExplosiveProjectile::SetDamage(float NewDamage)
{
	Damage = NewDamage;
}

UProjectileMovementComponent* ASGExplosiveProjectile::GetMovementComponent()
{
	return ProjectileMovement;
}

// Protected
void ASGExplosiveProjectile::BeginPlay()
{
	Super::BeginPlay();
	if (Mesh) Mesh->OnComponentHit.AddDynamic(this, &ASGExplosiveProjectile::OnHit);
}

// Private
void ASGExplosiveProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AActor* MyOwner = GetOwner();
	if (!MyOwner || OtherActor == this || OtherActor == MyOwner || OtherActor == MyOwner->GetOwner())
		return;

	// Spawn VFX/SFX
	DoSpecialEffects();

	// Optional: Debug sphere for explosion radius
	DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionRadius, 32, FColor::Orange, false, 2.0f);

	// Apply radial damage
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);
	if (MyOwner) IgnoredActors.Add(MyOwner);

	UGameplayStatics::ApplyRadialDamage(
		this,
		Damage,
		GetActorLocation(),
		ExplosionRadius,
		UDamageType::StaticClass(),
		IgnoredActors,
		this,
		GetInstigatorController(),
		true
	);

	Destroy();
}


void ASGExplosiveProjectile::DoSpecialEffects()
{
	if (CameraShakeClass) GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(CameraShakeClass);
	if (ExplodeSound) UGameplayStatics::PlaySoundAtLocation(this, ExplodeSound, GetActorLocation());
	if (ExplodeEffect) UGameplayStatics::SpawnEmitterAtLocation(this, ExplodeEffect, GetActorLocation(), GetActorRotation());
}