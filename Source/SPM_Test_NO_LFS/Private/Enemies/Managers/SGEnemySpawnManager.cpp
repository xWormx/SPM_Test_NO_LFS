// Joel Larsson Wendt || jola6902

#include "Enemies/Managers/SGEnemySpawnManager.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "SGEnemySpawnVolume.h"
#include "Enemies/Managers/SGEnemySpawnPoint.h"
#include "Enemies/Characters/SGEnemyCharacter.h"
#include "Player/SGPlayerCharacter.h"
#include "Utils/SGObjectPoolSubsystem.h"
#include "Core/SGObjectiveHandlerSubSystem.h"

ASGEnemySpawnManager::ASGEnemySpawnManager()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ASGEnemySpawnManager::BeginPlay()
{
    Super::BeginPlay();
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASGEnemySpawnPoint::StaticClass(), AllSpawnPoints);
    PlayerRef = Cast<ASGPlayerCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));

    ObjectiveHandlerSubSystem = GetWorld()->GetSubsystem<USGObjectiveHandlerSubSystem>();
    if (ObjectiveHandlerSubSystem)
    {
        ObjectiveHandlerSubSystem->OnObjectiveStartedWithType.AddDynamic(this, &ASGEnemySpawnManager::HandleMissionStart);
        ObjectiveHandlerSubSystem->OnObjectiveCompletedWithType.AddDynamic(this, &ASGEnemySpawnManager::HandleMissionEnd);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("SGEnemySpawnManager::BeginPlay() | ObjectiveHandlerSubSystem not found! Spawning will not respond to objectives."));
    }
}

void ASGEnemySpawnManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    UpdateDespawnChecks(DeltaTime);

    DrawDebug();
}

void ASGEnemySpawnManager::StartSpawning()
{
    bSpawningActive = true;
    StartSpawnTimer();
}

void ASGEnemySpawnManager::StopSpawning()
{
    bSpawningActive = false;
    GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
}

void ASGEnemySpawnManager::SetSpawnMode(ESpawnMode NewMode)
{
    SpawnMode = NewMode;
}

void ASGEnemySpawnManager::SetSpawnArea(int32 Index)
{
    SpawnVolumeIndex = Index;
}

void ASGEnemySpawnManager::SetEnemyCount(int32 Count)
{
    EnemyCountPerWave = Count;
}

void ASGEnemySpawnManager::SetMaxEnemies(int32 Max)
{
    MaxEnemiesAlive = Max;
}

void ASGEnemySpawnManager::StartSpawnTimer()
{
    GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &ASGEnemySpawnManager::OnSpawnTimerElapsed, TimeBetweenSpawns, false);
}

void ASGEnemySpawnManager::OnSpawnTimerElapsed()
{
    if (!bSpawningActive) return;
    if (EnemiesAlive < MaxEnemiesAlive)
    {
        SpawnEnemies();
    }
    StartSpawnTimer();
}

void ASGEnemySpawnManager::SpawnEnemies()
{
    TArray<AActor*> ValidPoints;
    
    FVector PlayerLocation = PlayerRef ? PlayerRef->GetActorLocation() : FVector::ZeroVector;

    for (AActor* PointActor : AllSpawnPoints)
    {
        ASGEnemySpawnPoint* SpawnPoint = Cast<ASGEnemySpawnPoint>(PointActor);
        if (!SpawnPoint) continue;

        bool bIsValid = false;

        switch (SpawnMode)
        {
            case ESpawnMode::Everywhere:
                bIsValid = true;
                break;

            case ESpawnMode::AtArea:
                if (SpawnVolumes.IsValidIndex(SpawnVolumeIndex) && SpawnVolumes[SpawnVolumeIndex])
                {
                    if (SpawnVolumes[SpawnVolumeIndex]->ContainedSpawnPoints.Contains(SpawnPoint))
                    {
                        bIsValid = true;
                    }
                }
                break;

            case ESpawnMode::AroundPlayer:
            {
                float Dist = FVector::Dist(SpawnPoint->GetActorLocation(), PlayerLocation);
                if (Dist >= MinDistanceFromPlayer && Dist <= SpawnRadiusAroundPlayer)
                {
                    bIsValid = true;
                }
                break;
            }

            default:
                break;
        }

        if (bIsValid)
        {
            ValidPoints.Add(SpawnPoint);
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("SGEnemySpawnManager::SpawnEnemies() | ValidPoints found: %i"), ValidPoints.Num());

    if (ValidPoints.Num() == 0) return;

    for (uint32 i = 0; i < EnemyCountPerWave && EnemiesAlive < MaxEnemiesAlive; ++i)
    {
        ASGEnemySpawnPoint* SpawnPoint = GetRandomSpawnPoint(ValidPoints);
        if (!SpawnPoint) continue;

        ASGEnemyCharacter* Enemy = SpawnPoint->SpawnEnemy(GetRandomEnemyType());
        if (!Enemy) continue;

        ++EnemiesAlive;
        if (!Enemy->OnEnemyDied.IsAlreadyBound(this, &ASGEnemySpawnManager::HandleEnemyDeath))
        {
            Enemy->OnEnemyDied.AddDynamic(this, &ASGEnemySpawnManager::HandleEnemyDeath);
        }

        CheckDespawnCandidate(Enemy);

        UE_LOG(LogTemp, Warning, TEXT("SGEnemySpawnManager::SpawnEnemies() | Enemy spawned. EnemiesAlive: %i / %i"), EnemiesAlive, MaxEnemiesAlive);
    }
}


ASGEnemySpawnPoint* ASGEnemySpawnManager::GetRandomSpawnPoint(const TArray<AActor*>& Points) const
{
    if (Points.Num() == 0) return nullptr;
    return Cast<ASGEnemySpawnPoint>(Points[FMath::RandRange(0, Points.Num() - 1)]);
}

TSubclassOf<ASGEnemyCharacter> ASGEnemySpawnManager::GetRandomEnemyType() const
{
    if (EnemyTypes.Num() == 0) return nullptr;
    return EnemyTypes[FMath::RandRange(0, EnemyTypes.Num() - 1)];
}

void ASGEnemySpawnManager::HandleEnemyDeath(ASGEnemyCharacter* DeadEnemy)
{
    EnemiesAlive = FMath::Max(0, EnemiesAlive - 1);
    DeadEnemy->OnEnemyDied.RemoveDynamic(this, &ASGEnemySpawnManager::HandleEnemyDeath);
    UE_LOG(LogTemp, Error, TEXT("SGEnemySpawnManager::HandleEnemyDeath() | Enemy died! EnemiesAlive: %i, MaxEnemiesAlive: %i."), EnemiesAlive, MaxEnemiesAlive);
}

void ASGEnemySpawnManager::UpdateDespawnChecks(float DeltaTime)
{
    for (int32 i = DespawnCandidates.Num() - 1; i >= 0; --i)
    {
        auto& Candidate = DespawnCandidates[i];
        if (!IsValid(Candidate.Enemy))
        {
            DespawnCandidates.RemoveAt(i);
            continue;
        }

        Candidate.TimeOutOfRange += DeltaTime;
        if (Candidate.TimeOutOfRange >= DespawnGracePeriod)
        {
            if (USGObjectPoolSubsystem* Pool = GetGameInstance()->GetSubsystem<USGObjectPoolSubsystem>())
            {
                Pool->ReturnObjectToPool(Candidate.Enemy);
            }
            DespawnCandidates.RemoveAt(i);
            EnemiesAlive = FMath::Max(0, EnemiesAlive - 1);
            UE_LOG(LogTemp, Error, TEXT("SGEnemySpawnManager::UpdateDespawnChecks() | Enemy culled by despawn checker! EnemiesAlive: %i, MaxEnemiesAlive: %i."), EnemiesAlive, MaxEnemiesAlive);
        }
    }
}

void ASGEnemySpawnManager::CheckDespawnCandidate(ASGEnemyCharacter* Enemy)
{
    if (!IsValid(Enemy) || !PlayerRef) return;

    float Distance = FVector::Dist(Enemy->GetActorLocation(), PlayerRef->GetActorLocation());
    if (Distance > SpawnRadiusAroundPlayer)
    {
        bool AlreadyTracked = DespawnCandidates.ContainsByPredicate([Enemy](const FDespawnCandidate& C) { return C.Enemy == Enemy; });
        if (!AlreadyTracked)
        {
            DespawnCandidates.Add({ Enemy, 0.f });
        }
    }
}

void ASGEnemySpawnManager::ClearAllEnemies()
{
    TArray<AActor*> FoundEnemies;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASGEnemyCharacter::StaticClass(), FoundEnemies);

    USGObjectPoolSubsystem* Pool = GetGameInstance()->GetSubsystem<USGObjectPoolSubsystem>();
    if (!Pool) return;

    for (AActor* EnemyActor : FoundEnemies)
    {
        if (ASGEnemyCharacter* Enemy = Cast<ASGEnemyCharacter>(EnemyActor))
        {
            Pool->ReturnObjectToPool(Enemy);
        }
    }

    EnemiesAlive = 0;
    DespawnCandidates.Empty();

    UE_LOG(LogTemp, Warning, TEXT("SGEnemySpawnManager::ClearAllEnemies() | All enemies returned to object pool."));
}

void ASGEnemySpawnManager::HandleMissionStart(EObjectiveType ObjectiveType)
{
    UE_LOG(LogTemp, Warning, TEXT("SGEnemySpawnManager::HandleMissionStart() | Function called."));
    
    SpawnVolumeIndex = MissionsCompleted;
    
    switch (MissionsCompleted)
    {
    case 0:
        MaxEnemiesAlive = 5;
        TimeBetweenSpawns = 3.f;
        SpawnMode = ESpawnMode::AtArea;
        break;
    case 1:
        MaxEnemiesAlive = 10;
        TimeBetweenSpawns = 2.f;
        SpawnMode = ESpawnMode::AtArea;
        ClearAllEnemies();
        break;
    case 2:
        MaxEnemiesAlive = 10;
        TimeBetweenSpawns = 1.f;
        SpawnMode = ESpawnMode::AtArea;
        ClearAllEnemies();
        break;
    case 3:
        MaxEnemiesAlive = 20;
        TimeBetweenSpawns = 0.75f;
        SpawnMode = ESpawnMode::AtArea;
        SpawnVolumeIndex = 2;
        break;
    }

    StartSpawning();
}

void ASGEnemySpawnManager::HandleMissionEnd(EObjectiveType ObjectiveType)
{
    MissionsCompleted++;
    StopSpawning();
}

void ASGEnemySpawnManager::DrawDebug()
{
    if (bDebugDraw && PlayerRef)
    {
        FVector PlayerLocation = PlayerRef->GetActorLocation();
        
        DrawDebugSphere(GetWorld(), PlayerLocation, SpawnRadiusAroundPlayer, 32, FColor::Green, false, 0.f, 0, 5.f);
        //DrawDebugSphere(GetWorld(), PlayerLocation, MinDistanceFromPlayer, 32, FColor::Red, false, 0.f, 0, 3.f);

        for (AActor* Point : AllSpawnPoints)
        {
            if (IsValid(Point))
            {
                DrawDebugBox(GetWorld(), Point->GetActorLocation(), FVector(50, 50, 50), FColor::Blue, false, 0.f, 0, 2.f);
                DrawDebugString(GetWorld(), Point->GetActorLocation() + FVector(0, 0, 100), TEXT("SpawnPoint"), nullptr, FColor::White, 0.f, false);
            }
        }
    }
}
