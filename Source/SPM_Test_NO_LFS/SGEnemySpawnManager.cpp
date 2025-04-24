#include "SGEnemySpawnManager.h"
#include "SGEnemySpawnPoint.h"
#include "SGEnemyCharacter.h"
#include "SGPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

// Public
ASGEnemySpawnManager::ASGEnemySpawnManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASGEnemySpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASGEnemySpawnManager::HandleEnemyDeath(ASGEnemyCharacter* DeadEnemy)
{
	--EnemiesAlive;
	//UE_LOG(LogTemp, Warning, TEXT("Enemies left: %i"), EnemiesAlive);

	if (EnemiesAlive <= 0)
	{
		StartIntermissionTimer();
	}
}

// Protected
void ASGEnemySpawnManager::BeginPlay()
{
	Super::BeginPlay();
	PlayerCharacter = Cast<ASGPlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("EnemySpawnPoint"), AllEnemySpawnPoints);
	//UE_LOG(LogTemp, Warning, TEXT("EnemySpawnPoints found: %i"), AllEnemySpawnPoints.Num());
	StartIntermissionTimer();
}

// Private
void ASGEnemySpawnManager::StartIntermissionTimer()
{
	UE_LOG(LogTemp, Warning, TEXT("Intermission timer started! %f seconds to next wave..."), TimeBetweenWaves);
	GetWorldTimerManager().SetTimer(IntermissionTimer, this, &ASGEnemySpawnManager::EndIntermissionTimer, TimeBetweenWaves, false);
}

void ASGEnemySpawnManager::EndIntermissionTimer()
{
	StartNextWave();
}

void ASGEnemySpawnManager::StartNextWave()
{
	++CurrentWave;
	SpawnEnemiesEverywhere();
}

void ASGEnemySpawnManager::SpawnEnemiesEverywhere()
{
	if (AllEnemySpawnPoints.Num() <= 0) return;

	TArray<AActor*> TempArray = AllEnemySpawnPoints;
	FVector PlayerLocation = PlayerCharacter->GetActorLocation();

	for (int32 i = TempArray.Num() - 1; i >= 0; --i)
	{
		AActor* EnemySpawnPoint = TempArray[i];
		float DistanceFromPlayer = FVector::Dist(EnemySpawnPoint->GetActorLocation(), PlayerLocation);
		if (DistanceFromPlayer < MinDistanceFromPlayer) TempArray.RemoveAt(i);
	}

	if (TempArray.Num() <= 0) return;

	int32 EnemyCount = FMath::CeilToInt(CurrentWave * EnemyCountScalingFactor);
	for (int32 i = 0; i < EnemyCount; ++i)
	{
		++EnemiesAlive;
		ASGEnemyCharacter* SpawnedEnemyPtr = GetRandomSpawnPoint(TempArray)->SpawnEnemy(GetRandomEnemyType());
		
		if (SpawnedEnemyPtr != nullptr)
		{
			SpawnedEnemyPtr->OnEnemyDied.AddDynamic(this, &ASGEnemySpawnManager::HandleEnemyDeath);
			if (SpawnSound) UGameplayStatics::PlaySoundAtLocation(GetWorld(), SpawnSound, SpawnedEnemyPtr->GetActorLocation());
		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("Wave %i started! %i enemies left..."), CurrentWave, EnemiesAlive);
}

const ASGEnemySpawnPoint* ASGEnemySpawnManager::GetRandomSpawnPoint(TArray<AActor*> SpawnPointArray) const
{
	return Cast<ASGEnemySpawnPoint>(SpawnPointArray[FMath::RandRange(0, SpawnPointArray.Num() - 1)]);
}

const TSubclassOf<ASGEnemyCharacter> ASGEnemySpawnManager::GetRandomEnemyType() const
{
	return EnemyTypes[FMath::RandRange(0, EnemyTypes.Num() - 1)];
}