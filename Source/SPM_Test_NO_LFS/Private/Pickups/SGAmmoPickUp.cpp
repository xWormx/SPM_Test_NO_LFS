#include "Pickups/SGAmmoPickUp.h"
#include "Player/SGPlayerCharacter.h"

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

	switch (AmmoType)
	{
		case EAmmoType::AssaultRifle:
			{
				// Do nothing
			}

		case EAmmoType::Shotgun:
			{
				
			}

		case EAmmoType::GrenadeLauncher:
			{
				
			}
	}
}

// Protected
void ASGAmmoPickUp::BeginPlay()
{
	Super::BeginPlay();
	PlayerCharacter = Cast<ASGPlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
}