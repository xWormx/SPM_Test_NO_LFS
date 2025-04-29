#include "SGPickUpObjectiveCollect.h"

#include "SGGameObjectivesHandler.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

ASGPickUpObjectiveCollect::ASGPickUpObjectiveCollect()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	Mesh->SetupAttachment(RootComponent);	
}

void ASGPickUpObjectiveCollect::BeginPlay()
{
	Super::BeginPlay();
	
	OnActorBeginOverlap.AddDynamic(this, &ASGPickUpObjectiveCollect::HandleOverlap);

	//TODO: Se över hantering av registrering collectables. Kompakt lösning för enkelheten just nu.
	ASGGameObjectivesHandler* GameObjectivesHandler = Cast<ASGGameObjectivesHandler>(
		UGameplayStatics::GetActorOfClass(GetWorld(), ASGGameObjectivesHandler::StaticClass()));

	if (!GameObjectivesHandler)
	{
		return;
	}
	
	GameObjectivesHandler->RegisterCollectible(this);
}

//TODO: Se över hantering av collectables. Kompakt lösning för enkelheten just nu.
void ASGPickUpObjectiveCollect::HandleOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (!OtherActor || OtherActor == GetOwner() || OtherActor == UGameplayStatics::GetPlayerPawn(GetWorld(), 0)) 
	{
		return;
	}	
	OnPickup();
}

void ASGPickUpObjectiveCollect::OnPickup_Implementation()
{	
	Super::OnPickup_Implementation(); // TODO: Implementera i en pool? Just nu släcks bara objektet 
	OnCollected.Broadcast(this);	
}

