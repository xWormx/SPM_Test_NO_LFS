#include "SGEnemySpawnPoint.h"
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

void ASGEnemySpawnPoint::SpawnEnemy() const
{
	if (EnemyClass == nullptr) return;

	UE_LOG(LogTemp, Warning, TEXT("%s spawned an enemy!"), *GetActorNameOrLabel());
	AActor* SpawnedEnemyPtr = GetWorld()->SpawnActor<AActor>(EnemyClass, GetActorLocation(), GetActorRotation());
	if (SpawnSound) UGameplayStatics::PlaySoundAtLocation(GetWorld(), SpawnSound, GetActorLocation());
}

// Protected
void ASGEnemySpawnPoint::BeginPlay()
{
	Super::BeginPlay();
}