#include "SGEnemySpawnManager.h"
#include "SGEnemySpawnPoint.h"
#include "SGEnemyCharacter.h"
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
	UE_LOG(LogTemp, Warning, TEXT("Enemies left: %i"), EnemiesAlive);

	if (EnemiesAlive <= 0)
	{
		StartIntermissionTimer();
	}
}

// Protected
void ASGEnemySpawnManager::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("EnemySpawnPoint"), EnemySpawnPoints);
	UE_LOG(LogTemp, Warning, TEXT("EnemySpawnPoints found: %i"), EnemySpawnPoints.Num());
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
	SpawnEnemies();
}

void ASGEnemySpawnManager::SpawnEnemies()
{
	if (EnemySpawnPoints.Num() <= 0) return;

	int32 EnemyCount = FMath::CeilToInt(CurrentWave * EnemyCountScalingFactor);
	for (int32 i = 0; i < EnemyCount; ++i)
	{
		++EnemiesAlive;
		ASGEnemyCharacter* SpawnedEnemyPtr = GetRandomSpawnPoint()->SpawnEnemy(GetRandomEnemyType());
		
		if (SpawnedEnemyPtr != nullptr)
		{
			SpawnedEnemyPtr->OnEnemyDied.AddDynamic(this, &ASGEnemySpawnManager::HandleEnemyDeath);
			if (SpawnSound) UGameplayStatics::PlaySoundAtLocation(GetWorld(), SpawnSound, SpawnedEnemyPtr->GetActorLocation());
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Wave %i started! %i enemies left..."), CurrentWave, EnemiesAlive);
}

const ASGEnemySpawnPoint* ASGEnemySpawnManager::GetRandomSpawnPoint() const
{
	return Cast<ASGEnemySpawnPoint>(EnemySpawnPoints[FMath::RandRange(0, EnemySpawnPoints.Num() - 1)]);
}

const TSubclassOf<ASGEnemyCharacter> ASGEnemySpawnManager::GetRandomEnemyType() const
{
	return EnemyTypes[FMath::RandRange(0, EnemyTypes.Num() - 1)];
}