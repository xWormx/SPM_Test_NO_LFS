#include "SGEnemySpawnManager.h"
#include "SGEnemySpawnPoint.h"
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
	UE_LOG(LogTemp, Warning, TEXT("Intermission timer started! 10... 9..."));
	GetWorldTimerManager().SetTimer(IntermissionTimer, this, &ASGEnemySpawnManager::EndIntermissionTimer, TimeBetweenWaves, false);
}

void ASGEnemySpawnManager::EndIntermissionTimer()
{
	UE_LOG(LogTemp, Warning, TEXT("Intermission timer ended!"));
	StartNextWave();
}

void ASGEnemySpawnManager::StartNextWave()
{
	++CurrentWave;
	SpawnEnemies();
	StartIntermissionTimer(); // Bör göras när waven är över (död)
}

void ASGEnemySpawnManager::SpawnEnemies()
{
	if (EnemySpawnPoints.Num() <= 0) return;

	for (int32 i = 0; i < CurrentWave * EnemyCountScalingFactor; ++i)
	{
		ASGEnemySpawnPoint* RandomSpawnPoint = Cast<ASGEnemySpawnPoint>(EnemySpawnPoints[FMath::RandRange(0, EnemySpawnPoints.Num() - 1)]);
		if (RandomSpawnPoint) RandomSpawnPoint->SpawnEnemy();
	}
}