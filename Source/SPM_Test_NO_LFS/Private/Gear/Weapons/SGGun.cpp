#include "Gear/Weapons/SGGun.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"

// Public
ASGGun::ASGGun()
{
 	PrimaryActorTick.bCanEverTick = true;
	//Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	//RootComponent = Root;
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	//Mesh->SetupAttachment(Root);
	SetRootComponent(Mesh);
}

void ASGGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASGGun::Fire()
{
	if (ShootParticles && Mesh) UGameplayStatics::SpawnEmitterAttached(ShootParticles, Mesh, TEXT("MuzzleFlashSocket"));
	if (ShootSound && Mesh) UGameplayStatics::SpawnSoundAttached(ShootSound, Mesh, TEXT("MuzzleFlashSocket"));

	FHitResult HitResult;
	FVector ShotDirection;
	if (HitScan(HitResult, ShotDirection))
	{
		DrawDebugPoint(GetWorld(), HitResult.Location, 10, FColor::Red, true);
		if (HitParticles) UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles, HitResult.Location, ShotDirection.Rotation());
		if (HitSound) UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, HitResult.Location);

		AActor* HitActor = HitResult.GetActor();
		if (HitActor != nullptr)
		{
			FPointDamageEvent DamageEvent(Damage, HitResult, ShotDirection, nullptr);
			HitActor->TakeDamage(Damage, DamageEvent, GetOwnerController(), this);
		}
	}

}

float ASGGun::GetFireRate() const
{
	return FireRate;
}

// Protected
void ASGGun::BeginPlay()
{
	Super::BeginPlay();
}

AController* ASGGun::GetOwnerController() const
{
	APawn* MyOwner = Cast<APawn>(GetOwner());
	if (MyOwner == nullptr) return nullptr;
	return MyOwner->GetController();
}

// Private
bool ASGGun::HitScan(FHitResult& OutHitResult, FVector& OutShotDirection)
{
	AController* OwnerController = GetOwnerController();
	if (OwnerController == nullptr) return false;
	
	FVector Location;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(Location, Rotation);
	//DrawDebugCamera(GetWorld(), Location, Rotation, 90, 3, FColor::Green, true);
	FVector End = Location + Rotation.Vector() * MaxRange;
	//DrawDebugLine(GetWorld(), Location, End, FColor::Green, true);
	FVector OutDirection = -Rotation.Vector();
	
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());
	return GetWorld()->LineTraceSingleByChannel(OutHitResult, Location, End, ECC_GameTraceChannel2, Params); //ECC_GameTraceChannel2 = Projectile (Blockas av pawns)
}