// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Components/SGEnemyProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ASGEnemyProjectile::ASGEnemyProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	this->SetActorScale3D(FVector(5, 5, 5));
	CollisionComponent->SetCollisionProfileName(TEXT("Projectile"));
	RootComponent = CollisionComponent;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));

	MeshComponent->SetupAttachment(CollisionComponent);
	

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = CollisionComponent;
	ProjectileMovement->InitialSpeed = MaxSpeed;
	ProjectileMovement->MaxSpeed = MaxSpeed;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	InitialLifeSpan = LifeSpan;

	Tags.Add("Projectile");
}

// Called when the game starts or when spawned
void ASGEnemyProjectile::BeginPlay()
{
	Super::BeginPlay();
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ASGEnemyProjectile::OnProjectileOverlap);
}

void ASGEnemyProjectile::OnProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Projectile") || OtherActor->ActorHasTag("Enemy"))
	{
		return;
	}

	if (OtherActor)
	{
		UGameplayStatics::ApplyDamage(OtherActor, Damage, GetOwner()->GetInstigatorController(), this, DamageTypeClass);
	}
}


