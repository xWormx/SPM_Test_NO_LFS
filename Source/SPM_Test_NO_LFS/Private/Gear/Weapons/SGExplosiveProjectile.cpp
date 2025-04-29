#include "../../../Public/Gear/Weapons/SGExplosiveProjectile.h"
#include "Kismet/GameplayStatics.h"

// Public
ASGExplosiveProjectile::ASGExplosiveProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

	// Physics setup
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
	Mesh->SetCollisionResponseToAllChannels(ECR_Block);
	Mesh->SetNotifyRigidBodyCollision(true);
	Mesh->SetSimulatePhysics(true);
	Mesh->SetEnableGravity(true);
	Mesh->SetMassOverrideInKg(NAME_None, ProjectileMass);
}

void ASGExplosiveProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASGExplosiveProjectile::SetDamage(float NewDamage)
{
	Damage = NewDamage;
}

// Protected
void ASGExplosiveProjectile::BeginPlay()
{
	Super::BeginPlay();
	if (Mesh) Mesh->OnComponentHit.AddDynamic(this, &ASGExplosiveProjectile::OnHit);
}

// Private
void ASGExplosiveProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AActor* MyOwner = GetOwner();
	if (!MyOwner || OtherActor == this || OtherActor == MyOwner || OtherActor == MyOwner->GetOwner())
	{
		return;
	}

	DoSpecialEffects();

	DrawDebugSphere(
		GetWorld(),
		HitComp->GetComponentLocation(),
		ExplosionRadius,
		30,
		FColor::Red,
		false,
		3.f
		);
	
	if (OtherActor && OtherActor != this && OtherActor != MyOwner)
	{
		UGameplayStatics::ApplyDamage(
			OtherActor,
			Damage,
			MyOwner->GetInstigatorController(),
			MyOwner,
			UDamageType::StaticClass()
		);

	}

	Destroy();
}

void ASGExplosiveProjectile::DoSpecialEffects()
{
	if (CameraShakeClass) GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(CameraShakeClass);
	if (ExplodeSound) UGameplayStatics::PlaySoundAtLocation(this, ExplodeSound, GetActorLocation());
	if (ExplodeEffect) UGameplayStatics::SpawnEmitterAtLocation(this, ExplodeEffect, GetActorLocation(), GetActorRotation());
}