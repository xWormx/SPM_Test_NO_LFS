#include "Enemies/Managers/SGEnemySpawnPoint.h"
#include "Enemies/Characters/SGEnemyCharacter.h"
#include "Objectives/SGGameObjectivesHandler.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BillboardComponent.h"

// Public
ASGEnemySpawnPoint::ASGEnemySpawnPoint()
{
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add(FName("EnemySpawnPoint"));

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	UBillboardComponent* Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	Billboard->SetupAttachment(RootComponent);
	
	//Billboard->SetRelativeScale3D(FVector(2.f));
	Billboard->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
	static ConstructorHelpers::FObjectFinder<UTexture2D> SpriteTexture(TEXT("/Game/Textures/T_EnemySpawnPointEditorOnly"));
	if (SpriteTexture.Succeeded()) Billboard->SetSprite(SpriteTexture.Object);
}

void ASGEnemySpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

ASGEnemyCharacter* ASGEnemySpawnPoint::SpawnEnemy(const TSubclassOf<ASGEnemyCharacter> EnemyClass) const
{
	if (EnemyClass == nullptr) return nullptr;

	ASGEnemyCharacter* SpawnedEnemyPtr;
	if (SpawnPointSpecificEnemyType)
	{
		SpawnedEnemyPtr = GetWorld()->SpawnActor<ASGEnemyCharacter>(
			SpawnPointSpecificEnemyType, GetActorLocation(), GetActorRotation());
	}
	else
	{
		SpawnedEnemyPtr = GetWorld()->SpawnActor<ASGEnemyCharacter>(
			EnemyClass, GetActorLocation(), GetActorRotation());
	}
	
	if (ObjectiveHandler)
	{
		ObjectiveHandler->RegisterEnemy(SpawnedEnemyPtr);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("EnemySpawnPoint: Must Assign ObjectiveHandler"));
	}
		
	return SpawnedEnemyPtr;
}

// Protected
void ASGEnemySpawnPoint::BeginPlay()
{
	Super::BeginPlay();
}