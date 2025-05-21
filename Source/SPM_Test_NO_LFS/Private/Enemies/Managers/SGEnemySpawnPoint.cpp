#include "Enemies/Managers/SGEnemySpawnPoint.h"

#include "SPM_Test_NO_LFS.h"
#include "Enemies/Characters/SGEnemyCharacter.h"
#include "Objectives/SGGameObjectivesHandler.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BillboardComponent.h"
#include "Core/SGObjectiveHandlerSubSystem.h"
#include "Utils/SGObjectPoolSubsystem.h"

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
	if (EnemyClass == nullptr)
	{
		return nullptr;
	}

	USGObjectPoolSubsystem* ObjectPool = GetGameInstance()->GetSubsystem<USGObjectPoolSubsystem>();

	TSubclassOf<AActor> EnemyType = TSubclassOf(SpawnPointSpecificEnemyType ? SpawnPointSpecificEnemyType : EnemyClass);
	AActor* PooledEnemy = ObjectPool->GetPooledObject(EnemyType);
	ASGEnemyCharacter* SpawnedEnemyPtr = Cast<ASGEnemyCharacter>(PooledEnemy);

	if (!ensureMsgf(SpawnedEnemyPtr, TEXT("Was not able to spawn enemy of type %s"), *EnemyType->GetName()))
	{
		return nullptr;
	}
	FVector ActorLocation = GetActorLocation();
	FVector ActorExtent;
	GetActorBounds(false, ActorLocation, ActorExtent);
	ActorLocation.Z += ActorExtent.Z/2.f;
	SpawnedEnemyPtr->SetActorLocationAndRotation(ActorLocation, GetActorRotation());

	/*
		if (ObjectiveHandler)
		{
			ObjectiveHandler->RegisterEnemy(SpawnedEnemyPtr);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("EnemySpawnPoint: Must Assign ObjectiveHandler"));
		}
	*/
	if (ObjectiveHandlerSubSystem)
	{
		ObjectiveHandlerSubSystem->RegisterEnemy(SpawnedEnemyPtr);
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
	ObjectiveHandlerSubSystem = GetWorld()->GetSubsystem<USGObjectiveHandlerSubSystem>();
	if (ObjectiveHandlerSubSystem == nullptr)
	{
		CALLE_LOG(Error, TEXT("SpawnPoint.cpp: ObjectiveHandlerSubSystem wasn't found!"));
	}
}