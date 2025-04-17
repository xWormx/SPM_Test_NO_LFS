#include "SGPickUp.h"

#include "SGUtilObjectPoolManager.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"


ASGPickUp::ASGPickUp()
{
	PrimaryActorTick.bCanEverTick = true;
	
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PickUpArea"));
	CollisionSphere->SetSphereRadius(100.0f);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionSphere->SetCollisionResponseToAllChannels(ECR_Overlap);
	RootComponent = CollisionSphere;
}

// Called when the game starts or when spawned
void ASGPickUp::BeginPlay()
{
	Super::BeginPlay();	
}


void ASGPickUp::OnPickup_Implementation()
{
	//TODO: Temporär lösning för enkelheten - ska ändras! 
	AActor* ObjectPoolActor = UGameplayStatics::GetActorOfClass(GetWorld(), ASGUtilObjectPoolManager::StaticClass());
	if (!ObjectPoolActor)
	{
		return;
	}
	ASGUtilObjectPoolManager* ObjectPoolManager = Cast<ASGUtilObjectPoolManager>(ObjectPoolActor);
	if (!ObjectPoolManager) 
	{
		Destroy(); // Ifall ObjectPoolManager inte existerar i leveln
		return;
	}
	ObjectPoolManager->ReturnObjectToPool(this);	
}

void ASGPickUp::SetPickUpValue(float Value)
{
	PickUpValue = Value;
}