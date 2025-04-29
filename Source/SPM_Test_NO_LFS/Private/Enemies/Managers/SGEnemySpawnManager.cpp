#include "Enemies/Managers/SGEnemySpawnManager.h"
#include "Enemies/Managers/SGEnemySpawnPoint.h"
#include "Enemies/Characters/SGEnemyCharacter.h"
#include "Player/SGPlayerCharacter.h"
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

void ASGEnemySpawnManager::StartSpawning()
{
	bSpawningIsActive = true;
	StartSpawnLoopTimer();
}

void ASGEnemySpawnManager::StopSpawning()
{
	bSpawningIsActive = false;
}

void ASGEnemySpawnManager::SetSpawnMode(ESpawnMode NewMode)
{
	SpawnMode = NewMode;
}

void ASGEnemySpawnManager::HandleEnemyDeath(ASGEnemyCharacter* DeadEnemy)
{
	--EnemiesAlive;
	//UE_LOG(LogTemp, Warning, TEXT("Enemies left: %i"), EnemiesAlive);
}

// Protected
void ASGEnemySpawnManager::BeginPlay()
{
	Super::BeginPlay();
	PlayerCharacter = Cast<ASGPlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("EnemySpawnPoint"), AllEnemySpawnPoints);
	//UE_LOG(LogTemp, Warning, TEXT("EnemySpawnPoints found: %i"), AllEnemySpawnPoints.Num());
	
	StartSpawning();
}

// Private
void ASGEnemySpawnManager::StartSpawnLoopTimer()
{
	//UE_LOG(LogTemp, Warning, TEXT("Intermission timer started! %f seconds to next wave..."), TimeBetweenSpawns);
	GetWorldTimerManager().SetTimer(SpawnLoopTimer, this, &ASGEnemySpawnManager::EndSpawnLoopTimer, TimeBetweenSpawns, false);
}

void ASGEnemySpawnManager::EndSpawnLoopTimer()
{
	if (!bSpawningIsActive) return;

	switch (SpawnMode)
	{
		case ESpawnMode::Everywhere:
			{
				SpawnEnemiesEverywhere();
				break;
			}
		
		case ESpawnMode::AtArea:
			{
				SpawnEnemiesAtArea();
				break;
			}
		
		case ESpawnMode::AroundPlayer:
			{
				SpawnEnemiesAroundPlayer();
				break;
			}
	}
	
	StartSpawnLoopTimer();
}

void ASGEnemySpawnManager::SetSpawnArea(uint32 Index)
{
	SpawnAreaIndex = Index;
}

void ASGEnemySpawnManager::SetEnemyCount(uint32 NewEnemyCount)
{
	EnemyCount = NewEnemyCount;
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

		if (MinDistanceFromPlayer >= SpawnRadiusAroundPlayer)
			UE_LOG(LogTemp, Error, TEXT("SpawnRadiusAroundPlayer must be larger than MinDistanceFromPlayer to allow spawning!"));
	}

	if (TempArray.Num() <= 0) return;
	
	SpawnEnemies(TempArray);
}

void ASGEnemySpawnManager::SpawnEnemiesAtArea()
{
	if (EnemySpawnPointGroups[SpawnAreaIndex].EnemySpawnPoints.Num() <= 0) return;

	TArray<AActor*> TempArray = EnemySpawnPointGroups[SpawnAreaIndex].EnemySpawnPoints;
	if (TempArray.Num() <= 0) return;
	
	SpawnEnemies(TempArray);
}

void ASGEnemySpawnManager::SpawnEnemiesAroundPlayer()
{
	if (AllEnemySpawnPoints.Num() <= 0) return;

	TArray<AActor*> TempArray = AllEnemySpawnPoints;
	FVector PlayerLocation = PlayerCharacter->GetActorLocation();

	for (int32 i = TempArray.Num() - 1; i >= 0; --i)
	{
		AActor* EnemySpawnPoint = TempArray[i];
		float DistanceFromPlayer = FVector::Dist(EnemySpawnPoint->GetActorLocation(), PlayerLocation);
		if (DistanceFromPlayer < MinDistanceFromPlayer || DistanceFromPlayer > SpawnRadiusAroundPlayer) TempArray.RemoveAt(i);
		
		if (MinDistanceFromPlayer >= SpawnRadiusAroundPlayer)
			UE_LOG(LogTemp, Error, TEXT("SpawnRadiusAroundPlayer must be larger than MinDistanceFromPlayer to allow spawning!"));
	}

	if (TempArray.Num() <= 0) return;
	
	SpawnEnemies(TempArray);
}

void ASGEnemySpawnManager::SpawnEnemies(const TArray<AActor*>& AvailableSpawnPoints)
{
	for (uint32 i = 0; i < EnemyCount; ++i)
	{
		ASGEnemyCharacter* SpawnedEnemyPtr = GetRandomSpawnPoint(AvailableSpawnPoints)->SpawnEnemy(GetRandomEnemyType());
		
		if (SpawnedEnemyPtr != nullptr)
		{
			++EnemiesAlive;
			SpawnedEnemyPtr->OnEnemyDied.AddDynamic(this, &ASGEnemySpawnManager::HandleEnemyDeath);
			if (SpawnSound) UGameplayStatics::PlaySoundAtLocation(GetWorld(), SpawnSound, SpawnedEnemyPtr->GetActorLocation());
		}
	}
}


const ASGEnemySpawnPoint* ASGEnemySpawnManager::GetRandomSpawnPoint(TArray<AActor*> SpawnPointArray) const
{
	return Cast<ASGEnemySpawnPoint>(SpawnPointArray[FMath::RandRange(0, SpawnPointArray.Num() - 1)]);
}

const TSubclassOf<ASGEnemyCharacter> ASGEnemySpawnManager::GetRandomEnemyType() const
{
	return EnemyTypes[FMath::RandRange(0, EnemyTypes.Num() - 1)];
}