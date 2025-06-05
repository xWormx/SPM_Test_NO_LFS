// Joel Larsson Wendt || jola6902

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemies/Managers/SGEnemySpawnPoint.h"
#include "Enemies/Characters/SGEnemyCharacter.h"
#include "Core/SGObjectiveHandlerSubSystem.h"
#include "SGEnemySpawnManager.generated.h"

USTRUCT(Blueprintable)
struct FSpawnManagerSavedData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    bool bSaveGameExists = false;

    UPROPERTY(EditAnywhere)
    int32 MissionsCompleted = 0;
};

UENUM(BlueprintType)
enum class ESpawnMode : uint8
{
    Everywhere,
    AtArea,
    AroundPlayer
};

/* Denna struct är till för att hålla koll på hur länge en fiende varit för långt ifrån spelaren för att kunna ta bort
fiender som fastnat eller är orelevanta för gameplay. Detta är istället för att varje fiende ska behöva ha en egen inne-
boende timer som sköter detta. */
USTRUCT()
struct FDespawnCandidate
{
    GENERATED_BODY()

    UPROPERTY()
    ASGEnemyCharacter* Enemy = nullptr;

    float TimeOutOfRange = 0.f;
};

UCLASS()
class ASGEnemySpawnManager : public AActor
{
    GENERATED_BODY()

public:
    ASGEnemySpawnManager();

    UFUNCTION(BlueprintCallable)
    FSpawnManagerSavedData GetSaveData() const;

    UFUNCTION(BlueprintCallable)
    void LoadSaveData(FSpawnManagerSavedData SaveData);

    virtual void Tick(float DeltaTime) override;
    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable)
    void StartSpawning();

    UFUNCTION(BlueprintCallable)
    void StopSpawning();

    UFUNCTION(BlueprintCallable)
    void ClearAllEnemies();

    UFUNCTION(BlueprintCallable)
    void SetSpawnMode(ESpawnMode NewMode);

    UFUNCTION(BlueprintCallable)
    void SetSpawnArea(int32 Index);

    UFUNCTION(BlueprintCallable)
    void SetEnemyCount(int32 Count);

    UFUNCTION(BlueprintCallable)
    void SetMaxEnemies(int32 Max);

    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="debug")
    bool bDebugDraw = true;

    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="design")
    float TimeBetweenSpawns = 3.f;

    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="design")
    int32 MaxEnemiesAlive = 10;

    // Number of enemies spawned per TimeBetweenSpawns
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="design")
    int32 EnemyCountPerWave = 1;

    // How long an enemy has to remain stuck / out of range before removal
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="design")
    float DespawnGracePeriod = 10.f;

    // Does not apply to ESpawnMode::AtArea
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="design")
    float MinDistanceFromPlayer = 300.f;

    // Does not apply to ESpawnMode::AtArea
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="design")
    float SpawnRadiusAroundPlayer = 1500.f;

    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="design")
    TArray<TSubclassOf<ASGEnemyCharacter>> EnemyTypes;

    // A list of ASGSpawnVolume* that defines the areas in ESpawnMode::AtArea
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="design")
    TArray<class ASGEnemySpawnVolume*> SpawnVolumes;

private:
    void StartSpawnTimer();
    void OnSpawnTimerElapsed();
    void SpawnEnemies();
    TArray<AActor*> FilterValidSpawnPoints(TArray<AActor*> SpawnPoints);
    ASGEnemySpawnPoint* GetRandomSpawnPoint(const TArray<AActor*>& Points) const;
    TSubclassOf<ASGEnemyCharacter> GetRandomEnemyType() const;

    UFUNCTION()
    void HandleEnemyDeath(ASGEnemyCharacter* DeadEnemy);
    
    void UpdateDespawnChecks(float DeltaTime);
    void CheckDespawnCandidate(ASGEnemyCharacter* Enemy);

    UFUNCTION()
    void HandleMissionStart(EObjectiveType ObjectiveType);
    
    UFUNCTION()
    void HandleMissionEnd(EObjectiveType ObjectiveType);

    void DrawDebug();

    UPROPERTY()
    TArray<AActor*> AllSpawnPoints;

    UPROPERTY()
    TArray<FDespawnCandidate> DespawnCandidates;

    UPROPERTY()
    FTimerHandle SpawnTimerHandle;

    UPROPERTY()
    int32 EnemiesAlive = 0;

    UPROPERTY()
    bool bSpawningActive = false;
    

    ESpawnMode SpawnMode = ESpawnMode::Everywhere;
    uint32 SpawnVolumeIndex = 0;

    UPROPERTY()
    class ASGPlayerCharacter* PlayerRef = nullptr;

    UPROPERTY()
    USGObjectiveHandlerSubSystem* ObjectiveHandlerSubSystem = nullptr;

    int32 MissionsCompleted = 0;

    UPROPERTY(EditAnywhere)
    class ASGVoiceLines* VoiceLineManager = nullptr;

    UPROPERTY(EditAnywhere)
    class ASGMusicPlayer* MusicPlayer = nullptr;
};