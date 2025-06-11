// EnemySpawner.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/TimerHandle.h"
#include "EnemySpawner.generated.h"

UCLASS(BlueprintType, Blueprintable)
class FPSGAME_API AEnemySpawner : public AActor
{
    GENERATED_BODY()

public:
    AEnemySpawner();

protected:
    virtual void BeginPlay() override;

    // Enemy class to spawn
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
    TSubclassOf<APawn> EnemyClass;

    // Spawn interval in seconds
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning", meta = (ClampMin = "0.1"))
    float SpawnInterval = 2.0f;

    // Maximum number of enemies to spawn (0 = infinite)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning", meta = (ClampMin = "0"))
    int32 MaxEnemies = 0;

    // Spawn radius around the spawner
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning", meta = (ClampMin = "0"))
    float SpawnRadius = 500.0f;

    // Whether spawning is currently active
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
    bool bIsSpawningActive = true;

    // Current number of enemies spawned
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
    int32 CurrentEnemyCount = 0;

private:
    // Timer handle for spawning
    FTimerHandle SpawnTimerHandle;

    // Array to keep track of spawned enemies
    UPROPERTY()
    TArray<APawn*> SpawnedEnemies;

public:
    // Blueprint callable functions
    UFUNCTION(BlueprintCallable, Category = "Spawning")
    void StartSpawning();

    UFUNCTION(BlueprintCallable, Category = "Spawning")
    void StopSpawning();

    UFUNCTION(BlueprintCallable, Category = "Spawning")
    void SpawnEnemy();

    UFUNCTION(BlueprintCallable, Category = "Spawning")
    void ClearAllEnemies();

    // Blueprint implementable events
    UFUNCTION(BlueprintImplementableEvent, Category = "Spawning")
    void OnEnemySpawned(APawn* SpawnedEnemy);

    UFUNCTION(BlueprintImplementableEvent, Category = "Spawning")
    void OnMaxEnemiesReached();

private:
    void SpawnEnemyInternal();
    void CleanupDestroyedEnemies();
    FVector GetRandomSpawnLocation();
};
