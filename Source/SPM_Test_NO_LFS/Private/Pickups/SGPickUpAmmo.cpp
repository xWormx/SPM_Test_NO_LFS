#include "Pickups/SGPickUpAmmo.h"
#include "Gear/Weapons/SGGun.h"

ASGPickUpAmmo::ASGPickUpAmmo()
{
	AmmoMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AmmoMesh"));
	AmmoMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AmmoMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	AmmoMesh->SetupAttachment(RootComponent);

	PickUpValue = 1;
}

bool ASGPickUpAmmo::IsCompatibleWithGun(const TSubclassOf<ASGGun>& Gun) const
{
	//Reminder to self... Om flera vapen kommer någon gång kunna använda samma ammo - kör for-loopen som checkar om GunType == Gun här istället för en GetCompatibleGuns t.ex.
	return GunType == Gun;
}