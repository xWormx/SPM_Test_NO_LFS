#include "Pickups/SGAmmoPickUp.h"
#include "Player/SGPlayerCharacter.h"
#include "Gear/Weapons/SGGun.h"

// Public
ASGAmmoPickUp::ASGAmmoPickUp()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	Mesh->SetupAttachment(RootComponent);
}

void ASGAmmoPickUp::OnPickup_Implementation()
{
	Super::OnPickup_Implementation();

	TArray<ASGGun*> Guns = PlayerCharacter->GetGuns();
	if (Guns.IsEmpty()) return;
	
	switch (AmmoType)
	{
		case EAmmoType::AssaultRifle:
			{
				// AssaultRifle has infinite ammo (do nothing)
			}

		case EAmmoType::Shotgun:
			{
				if (Guns[1] && Guns.Num() > 1)
				{
					Guns[1]->AddAmmo(AmmoAmount);
				}
			}

		case EAmmoType::GrenadeLauncher:
			{
				if (Guns[2] && Guns.Num() > 2)
				{
					Guns[2]->AddAmmo(AmmoAmount);
				}
			}
	}
}

// Protected
void ASGAmmoPickUp::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerCharacter = Cast<ASGPlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
}