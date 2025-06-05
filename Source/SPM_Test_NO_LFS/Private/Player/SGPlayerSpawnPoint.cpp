


#include "Player/SGPlayerSpawnPoint.h"

#include "Components/BillboardComponent.h"


ASGPlayerSpawnPoint::ASGPlayerSpawnPoint()
{

	PrimaryActorTick.bCanEverTick = false;

	Tags.Add(TEXT("PlayerSpawnPoint"));

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	Billboard->SetupAttachment(Root);

	static ConstructorHelpers::FObjectFinder<UTexture2D> Icon(TEXT("/Game/Textures/T_PlayerSpawnPointEditorOnly"));

	if (Icon.Succeeded())
	{
		Billboard->SetSprite(Icon.Object);
	}

	bIsEditorOnlyActor = false;
	
}


void ASGPlayerSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

