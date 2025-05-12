#include "Enemies/Managers/SGEnemySpawnManager.h"
#include "Components/SphereComponent.h"
#include "Enemies/Managers/SGEnemySpawnPoint.h"
#include "Enemies/Characters/SGEnemyCharacter.h"
#include "Player/SGPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Objectives/SGObjectiveDefendThePod.h"
#include "Utils/SGObjectPoolSubsystem.h"

// Public functions
ASGEnemySpawnManager::ASGEnemySpawnManager()
{
	PrimaryActorTick.bCanEverTick = true;

	DespawnTriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DespawnTriggerSphere"));
	DespawnTriggerSphere->SetupAttachment(RootComponent);
	DespawnTriggerSphere->SetSphereRadius(DespawnTriggerRadius);
	DespawnTriggerSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DespawnTriggerSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	DespawnTriggerSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	DespawnTriggerSphere->SetGenerateOverlapEvents(true);
	DespawnTriggerSphere->SetHiddenInGame(true);
	DespawnTriggerSphere->bDrawOnlyIfSelected = false;
	DespawnTriggerSphere->SetVisibility(true);
	DespawnTriggerSphere->ShapeColor = FColor::Green;
}

void ASGEnemySpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (int32 i = DespawnCandidates.Num() - 1; i >= 0; --i)
	{
		FDespawnCandidate& Candidate = DespawnCandidates[i];
		
		if (!IsValid(Candidate.Enemy))
		{
			DespawnCandidates.RemoveAt(i);
			continue;
		}

		Candidate.TimeOutOfRange += DeltaTime;

		if (Candidate.TimeOutOfRange >= DespawnGracePeriod)
		{
			GetGameInstance()->GetSubsystem<USGObjectPoolSubsystem>()->ReturnObjectToPool(Candidate.Enemy);
			EnemiesAlive--;
			DespawnCandidates.RemoveAt(i);
			
			UE_LOG(LogTemp, Warning, TEXT("EnemySpawnManager::An enemy has been culled by the despawn checker!"));
			UE_LOG(LogTemp, Warning, TEXT("EnemySpawnManager::EnemiesAlive[%i],MaxEnemiesAtATime[%i]"), EnemiesAlive, MaxEnemiesAlive);
		}
	}
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
	UE_LOG(LogTemp, Warning, TEXT("EnemySpawnManager::EnemiesAlive[%i],MaxEnemiesAtATime[%i]"), EnemiesAlive, MaxEnemiesAlive);

	GetGameInstance()->GetSubsystem<USGObjectPoolSubsystem>()->ReturnObjectToPool(DeadEnemy);
}

// Protected functions
void ASGEnemySpawnManager::BeginPlay()
{
	Super::BeginPlay();
	DefaultSpawnMode = SpawnMode;
	DefaultMaxEnemiesAlive = MaxEnemiesAlive;
	DefaultTimeBetweenSpawns = TimeBetweenSpawns;
	PlayerCharacter = Cast<ASGPlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("EnemySpawnPoint"), AllEnemySpawnPoints);
	//UE_LOG(LogTemp, Warning, TEXT("EnemySpawnPoints found: %i"), AllEnemySpawnPoints.Num());

	DespawnTriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &ASGEnemySpawnManager::HandleBeginOverlap);
	DespawnTriggerSphere->OnComponentEndOverlap.AddDynamic(this, &ASGEnemySpawnManager::HandleEndOverlap);
	
	if (ObjectiveDefendThePod)
	{
		ObjectiveDefendThePod->OnDefendEventStart.AddDynamic(this, &ASGEnemySpawnManager::HandleDefendEventStart);
		ObjectiveDefendThePod->OnDefendEventEnd.AddDynamic(this, &ASGEnemySpawnManager::HandleDefendEventEnd);
	}
	
	StartSpawning();
}

// Private functions
void ASGEnemySpawnManager::StartSpawnLoopTimer()
{
	//UE_LOG(LogTemp, Warning, TEXT("Intermission timer started! %f seconds to next wave..."), TimeBetweenSpawns);
	GetWorldTimerManager().SetTimer(SpawnLoopTimer, this, &ASGEnemySpawnManager::EndSpawnLoopTimer, TimeBetweenSpawns, false);
	
	if (PlayerCharacter && DespawnTriggerSphere)
	{
		DespawnTriggerSphere->SetSphereRadius(DespawnTriggerRadius);
		DespawnTriggerSphere->SetWorldLocation(PlayerCharacter->GetActorLocation());
	}
}

void ASGEnemySpawnManager::EndSpawnLoopTimer()
{
	if (!bSpawningIsActive) return;

	switch (SpawnMode)
	{
		case ESpawnMode::Everywhere:
			{
				MaxEnemiesAlive = DefaultMaxEnemiesAlive;
				TimeBetweenSpawns = DefaultTimeBetweenSpawns;
				if (EnemiesAlive >= MaxEnemiesAlive) break;
				SpawnEnemiesEverywhere();
				break;
			}
		
		case ESpawnMode::AtArea:
			{
				MaxEnemiesAlive = DefaultMaxEnemiesAlive;
				TimeBetweenSpawns = DefaultTimeBetweenSpawns;
				if (EnemiesAlive >= MaxEnemiesAlive) break;
				SpawnEnemiesAtArea();
				break;
			}
		
		case ESpawnMode::AroundPlayer:
			{
				MaxEnemiesAlive *= MaxEnemiesAliveDefendThePodMultiplier;
				TimeBetweenSpawns = 0.5f;
				if (EnemiesAlive >= MaxEnemiesAlive) break;
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

void ASGEnemySpawnManager::SetMaxEnemiesAtATime(int32 NewMaxEnemiesAtATime)
{
	MaxEnemiesAlive = NewMaxEnemiesAtATime;
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
			UE_LOG(LogTemp, Warning, TEXT("EnemySpawnManager::EnemiesAlive[%i],MaxEnemiesAtATime[%i]"), EnemiesAlive, MaxEnemiesAlive);
			if (SpawnSound) UGameplayStatics::PlaySoundAtLocation(GetWorld(), SpawnSound, SpawnedEnemyPtr->GetActorLocation());
			CheckIfDespawnCandidate(SpawnedEnemyPtr);
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

void ASGEnemySpawnManager::CheckIfDespawnCandidate(ASGEnemyCharacter* Enemy)
{
	if (!IsValid(Enemy) || !DespawnTriggerSphere) return;
	
	if (!DespawnTriggerSphere->IsOverlappingActor(Enemy))
	{
		bool bAlreadyTracked = DespawnCandidates.ContainsByPredicate([Enemy](const FDespawnCandidate& Candidate)
		{
			return Candidate.Enemy == Enemy;
		});

		if (!bAlreadyTracked)
		{
			FDespawnCandidate Candidate;
			Candidate.Enemy = Enemy;
			Candidate.TimeOutOfRange = 0.f;
			DespawnCandidates.Add(Candidate);

			UE_LOG(LogTemp, Warning, TEXT("EnemySpawnManager::[%s] has spawned outside the despawn checker area!"), *Enemy->GetName());
		}
	}
}

void ASGEnemySpawnManager::HandleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ASGEnemyCharacter* Enemy = Cast<ASGEnemyCharacter>(OtherActor))
	{
		DespawnCandidates.RemoveAll([Enemy](const FDespawnCandidate& Candidate)
		{
			return Candidate.Enemy == Enemy;
		});
	}
}

void ASGEnemySpawnManager::HandleEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ASGEnemyCharacter* Enemy = Cast<ASGEnemyCharacter>(OtherActor))
	{
		FDespawnCandidate Candidate;
		Candidate.Enemy = Enemy;
		Candidate.TimeOutOfRange = 0.f;
		DespawnCandidates.Add(Candidate);
	}
}

void ASGEnemySpawnManager::HandleDefendEventStart()
{
	if (EnemySpawnPointGroups.Num() <= 0) return;
	
	SpawnMode = ESpawnMode::AtArea;
	SpawnAreaIndex = 0;
}

void ASGEnemySpawnManager::HandleDefendEventEnd(UObject* ObjectiveInterfaceImplementor)
{
	SpawnMode = DefaultSpawnMode;
}