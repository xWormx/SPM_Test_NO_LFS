#include "../../../Public/Gear/Weapons/SGExplosiveProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Public
ASGExplosiveProjectile::ASGExplosiveProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
	SphereCollider->SetupAttachment(Root);
	SphereCollider->InitSphereRadius(30.0f);
	SphereCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCollider->SetCollisionResponseToAllChannels(ECR_Overlap);
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 1000.f;
	ProjectileMovement->MaxSpeed = 5000.f;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->Bounciness = 0.6f;
	ProjectileMovement->ProjectileGravityScale = 1.0f;
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
	if (SphereCollider) SphereCollider->OnComponentBeginOverlap.AddDynamic(this, &ASGExplosiveProjectile::OnBeginOverlap);
}

// Private
void ASGExplosiveProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AActor* MyOwner = GetOwner();
	AActor* MyOwnersOwner = GetOwner()->GetOwner();
	if (!MyOwner || OtherActor == this || OtherActor == MyOwner || OtherActor == MyOwnersOwner)
		return;
	
	DoSpecialEffects();
	
	DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionRadius, 32, FColor::Orange, false, 2.0f);
	
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);
	if (MyOwner) IgnoredActors.Add(MyOwner);
	if (MyOwnersOwner) IgnoredActors.Add(MyOwnersOwner);

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
	if (ExplosionDecal)
	{
		FVector Location = GetActorLocation();
		FVector Normal = FVector::UpVector;
		FHitResult Hit;

		FVector Start = Location;
		FVector End = Start - FVector(0, 0, ExplosionDecalRange);

		if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility))
		{
			Normal = Hit.ImpactNormal;
			
			float RandomYaw = FMath::FRandRange(0.f, 360.f);
			FRotator DecalRotation = Normal.Rotation();
			DecalRotation.Yaw += RandomYaw;

			UGameplayStatics::SpawnDecalAtLocation(
				GetWorld(),
				ExplosionDecal,
				FVector(30.f, ExplosionDecalSize, ExplosionDecalSize),
				Hit.ImpactPoint,
				DecalRotation,
				ExplosionDecalLifetime
			);
		}
	}
}