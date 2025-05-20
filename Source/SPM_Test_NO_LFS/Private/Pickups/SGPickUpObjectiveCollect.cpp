#include "Pickups/SGPickUpObjectiveCollect.h"

#include "Objectives/SGGameObjectivesHandler.h"
#include "Components/StaticMeshComponent.h"
#include "Core/SGObjectiveHandlerSubSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Utils/SGObjectPoolSubsystem.h"

ASGPickUpObjectiveCollect::ASGPickUpObjectiveCollect()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	Mesh->SetupAttachment(RootComponent);	
}

void ASGPickUpObjectiveCollect::ReturnToStartLocation()
{
	SetActorLocation(InitialPosition);
}

void ASGPickUpObjectiveCollect::BeginPlay()
{
	Super::BeginPlay();
	
	OnActorBeginOverlap.AddDynamic(this, &ASGPickUpObjectiveCollect::HandleOverlap);

	//TODO: Se över hantering av registrering collectables. Kompakt lösning för enkelheten just nu.
	/*
	 ASGGameObjectivesHandler* GameObjectivesHandler = Cast<ASGGameObjectivesHandler>(
		UGameplayStatics::GetActorOfClass(GetWorld(), ASGGameObjectivesHandler::StaticClass()));
	if (!GameObjectivesHandler)
	{
		return;
	}

	GameObjectivesHandler->RegisterCollectible(this);
	*/
	USGObjectiveHandlerSubSystem* ObjectiveHandlerSubSystem = GetWorld()->GetSubsystem<USGObjectiveHandlerSubSystem>();
	if (!ObjectiveHandlerSubSystem)
	{
		return;
	}

	ObjectiveHandlerSubSystem->RegisterCollectible(this);

	InitialPosition = GetActorLocation();

	FTimerHandle DelayTimer;
	GetWorldTimerManager().SetTimer(DelayTimer, FTimerDelegate::CreateLambda([this]()
	{
		GetGameInstance()->GetSubsystem<USGObjectPoolSubsystem>()->ReturnObjectToPool(this);
	}), 0.1f, false);
}

//TODO: Se över hantering av collectables. Kompakt lösning för enkelheten just nu.
void ASGPickUpObjectiveCollect::HandleOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (!OtherActor || OtherActor == GetOwner() || OtherActor != UGameplayStatics::GetPlayerPawn(GetWorld(), 0)) 
	{
		return;
	}	
	OnPickup();
}

void ASGPickUpObjectiveCollect::OnPickup_Implementation()
{	
	Super::OnPickup_Implementation(); // TODO: Implementera i en pool? Just nu släcks bara objektet
	UGameplayStatics::PlaySound2D(this, PickUpSound);
	OnCollected.Broadcast(this);	
}

