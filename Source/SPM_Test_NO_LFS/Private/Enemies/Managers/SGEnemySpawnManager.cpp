// Joel Larsson Wendt || jola6902

#include "Enemies/Managers/SGEnemySpawnManager.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Enemies/Managers/SGEnemySpawnVolume.h"
#include "Enemies/Managers/SGEnemySpawnPoint.h"
#include "Enemies/Characters/SGEnemyCharacter.h"
#include "Player/SGPlayerCharacter.h"
#include "Utils/SGObjectPoolSubsystem.h"
#include "Core/SGObjectiveHandlerSubSystem.h"
#include "Sound/SGVoiceLines.h"

ASGEnemySpawnManager::ASGEnemySpawnManager()
{
    PrimaryActorTick.bCanEverTick = true;
}

// Sätter upp referenser och bindings
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
        SpawnMode = ESpawnMode::AroundPlayer;
    }
}

void ASGEnemySpawnManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    UpdateDespawnChecks(DeltaTime);

    DrawDebug();
}

// Publikt API (C++ och BP) start
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
// Publikt API (C++ och BP) slut

// StartSpawnTimer ihop med OnSpawnTimerElapsed utgör en kontinuerlig timerbaserad loop som fortlöper tills den får order om att sluta
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

// Hämtar ut fiender från object poolen, sålänge inte MaxEnemiesAlive är uppnått, och binder fiende-death hantering till dem.
void ASGEnemySpawnManager::SpawnEnemies()
{
    TArray<AActor*> ValidPoints = FilterValidSpawnPoints(AllSpawnPoints);

    if (ValidPoints.Num() == 0) return;

    for (int32 i = 0; i < EnemyCountPerWave && EnemiesAlive < MaxEnemiesAlive; ++i)
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

        if (VoiceLineManager)
        {
            Enemy->OnEnemyDied.AddUniqueDynamic(VoiceLineManager, &ASGVoiceLines::Voice_Fluff);
        }

        CheckDespawnCandidate(Enemy);

        UE_LOG(LogTemp, Warning, TEXT("SGEnemySpawnManager::SpawnEnemies() | Enemy spawned. EnemiesAlive: %i / %i"), EnemiesAlive, MaxEnemiesAlive);
    }
}

// Väljer, baserat på SpawnMode, vilka SGEnemySpawnPoints som är OK att spawna ifrån och returnerar en TArray med dessa.
TArray<AActor*> ASGEnemySpawnManager::FilterValidSpawnPoints(TArray<AActor*> SpawnPoints)
{
    TArray<AActor*> ValidPoints;
    
    FVector PlayerLocation = PlayerRef ? PlayerRef->GetActorLocation() : FVector::ZeroVector;

    for (AActor* PointActor : SpawnPoints)
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

    return ValidPoints;
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
    EnemiesAlive = FMath::Max(0, EnemiesAlive - 1); // Hade innan refactor problem med att EnemiesAlive kunde bli ett negativt heltal, problemet är löst men denna finns kvar som en säkerhetsbarriär.
    DeadEnemy->OnEnemyDied.RemoveDynamic(this, &ASGEnemySpawnManager::HandleEnemyDeath);
    if (VoiceLineManager) DeadEnemy->OnEnemyDied.RemoveDynamic(VoiceLineManager, &ASGVoiceLines::Voice_Fluff);
    UE_LOG(LogTemp, Error, TEXT("SGEnemySpawnManager::HandleEnemyDeath() | Enemy died! EnemiesAlive: %i, MaxEnemiesAlive: %i."), EnemiesAlive, MaxEnemiesAlive);
}

// Uppdaterar tiden som en DespawnCandidate (se: struct i .h-filen) varit utanför tillåten räckvidd, och skickar tillbaka dem till object poolen ifall de varit frånvarande för länge
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
                if (VoiceLineManager) Candidate.Enemy->OnEnemyDied.RemoveDynamic(VoiceLineManager, &ASGVoiceLines::Voice_Fluff);
            }
            DespawnCandidates.RemoveAt(i);
            EnemiesAlive = FMath::Max(0, EnemiesAlive - 1);
            UE_LOG(LogTemp, Error, TEXT("SGEnemySpawnManager::UpdateDespawnChecks() | Enemy culled by despawn checker! EnemiesAlive: %i, MaxEnemiesAlive: %i."), EnemiesAlive, MaxEnemiesAlive);
        }
    }
}

/* Används vid spawn för att kolla om en fiende spawnat utanför tillåten räckvidd. (Jobbar på en lösning där en
triggervolym avgör med Begin/EndOverlap vilka fiender som ska markeras och avmarkeras) för att kunna ta bort så många
relevanta fiender som möjligt, men den lösningen ledde till ett antal buggar så den är inte med just nu. Hade kunnat
köra en version av denna med Dist() mot samtliga fiender i Tick() men det kändes inte rätt då det blir väldigt
dyrt helt i onödan, jag tror snarare på min tidigare nämnda lösning. */
void ASGEnemySpawnManager::CheckDespawnCandidate(ASGEnemyCharacter* Enemy)
{
    if (!IsValid(Enemy) || !PlayerRef) return;

    /*
    float Distance = FVector::Dist(Enemy->GetActorLocation(), PlayerRef->GetActorLocation());
    if (Distance > SpawnRadiusAroundPlayer)
    {
        bool AlreadyTracked = DespawnCandidates.ContainsByPredicate([Enemy](const FDespawnCandidate& C) { return C.Enemy == Enemy; });
        if (!AlreadyTracked)
        {
            DespawnCandidates.Add({ Enemy, 0.f });
        }
    }
    */
}

// Skickar tillbaka samtliga fiender till object poolen
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
            if (VoiceLineManager) Enemy->OnEnemyDied.RemoveDynamic(VoiceLineManager, &ASGVoiceLines::Voice_Fluff);
        }
    }

    EnemiesAlive = 0;
    DespawnCandidates.Empty();

    UE_LOG(LogTemp, Warning, TEXT("SGEnemySpawnManager::ClearAllEnemies() | All enemies returned to object pool."));
}

// Hårdkodad version av denna manager för att lösa vår senaste plan om hur spelet ska vara uppbyggt.
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
