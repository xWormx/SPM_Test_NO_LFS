#include "Pickups/SGPickUpHealthPack.h"


ASGPickUpHealthPack::ASGPickUpHealthPack()
{
	HealthPacketMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HealthPacketMesh"));
	HealthPacketMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HealthPacketMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	HealthPacketMesh->SetupAttachment(RootComponent);	
}
