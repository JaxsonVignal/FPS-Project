// EnemySpawner.cpp
#include "EnemySpawner.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

AEnemySpawner::AEnemySpawner()
{
    PrimaryActorTick.bCanEverTick = false;

    // Create a simple mesh component for visualization in editor
    UStaticMeshComponent* MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpawnerMesh"));
    RootComponent = MeshComponent;

    // Set default mesh (cube)
    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset(TEXT("/Engine/BasicShapes/Cube"));
    if (CubeMeshAsset.Succeeded())
    {
        MeshComponent->SetStaticMesh(CubeMeshAsset.Object);
        MeshComponent->SetWorldScale3D(FVector(0.5f, 0.5f, 0.5f));
    }
}

void AEnemySpawner::BeginPlay()
{
    Super::BeginPlay();

    if (bIsSpawningActive)
    {
        StartSpawning();
    }
}

void AEnemySpawner::StartSpawning()
{
    if (!EnemyClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("EnemySpawner: No enemy class set!"));
        return;
    }

    bIsSpawningActive = true;

    // Clear any existing timer
    GetWorldTimerManager().ClearTimer(SpawnTimerHandle);

    // Start the spawning timer
    GetWorldTimerManager().SetTimer(
        SpawnTimerHandle,
        this,
        &AEnemySpawner::SpawnEnemyInternal,
        SpawnInterval,
        true // Loop
    );

    UE_LOG(LogTemp, Log, TEXT("EnemySpawner: Started spawning"));
}

void AEnemySpawner::StopSpawning()
{
    bIsSpawningActive = false;
    GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
    UE_LOG(LogTemp, Log, TEXT("EnemySpawner: Stopped spawning"));
}

void AEnemySpawner::SpawnEnemy()
{
    SpawnEnemyInternal();
}

void AEnemySpawner::SpawnEnemyInternal()
{
    // Clean up destroyed enemies first
    CleanupDestroyedEnemies();

    // Check if we've reached the maximum enemy count
    if (MaxEnemies > 0 && CurrentEnemyCount >= MaxEnemies)
    {
        StopSpawning();
        OnMaxEnemiesReached();
        return;
    }

    if (!EnemyClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("EnemySpawner: No enemy class set!"));
        return;
    }

    // Get spawn location
    FVector SpawnLocation = GetRandomSpawnLocation();
    FRotator SpawnRotation = FRotator::ZeroRotator;

    // Spawn parameters
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.Instigator = GetInstigator();
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    // Spawn the enemy
    APawn* SpawnedEnemy = GetWorld()->SpawnActor<APawn>(EnemyClass, SpawnLocation, SpawnRotation, SpawnParams);

    if (SpawnedEnemy)
    {
        SpawnedEnemies.Add(SpawnedEnemy);
        CurrentEnemyCount++;

        UE_LOG(LogTemp, Log, TEXT("EnemySpawner: Spawned enemy at location %s"), *SpawnLocation.ToString());

        // Call Blueprint event
        OnEnemySpawned(SpawnedEnemy);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("EnemySpawner: Failed to spawn enemy"));
    }
}

void AEnemySpawner::CleanupDestroyedEnemies()
{
    // Remove null/destroyed enemies from the array
    SpawnedEnemies.RemoveAll([](APawn* Enemy) { return !IsValid(Enemy); });
    CurrentEnemyCount = SpawnedEnemies.Num();
}

FVector AEnemySpawner::GetRandomSpawnLocation()
{
    FVector BaseLocation = GetActorLocation();

    // Generate random point in circle
    float Angle = FMath::RandRange(0.0f, 2.0f * PI);
    float Distance = FMath::RandRange(0.0f, SpawnRadius);

    FVector Offset = FVector(
        FMath::Cos(Angle) * Distance,
        FMath::Sin(Angle) * Distance,
        0.0f
    );

    return BaseLocation + Offset;
}

void AEnemySpawner::ClearAllEnemies()
{
    // Destroy all spawned enemies
    for (APawn* Enemy : SpawnedEnemies)
    {
        if (IsValid(Enemy))
        {
            Enemy->Destroy();
        }
    }

    SpawnedEnemies.Empty();
    CurrentEnemyCount = 0;

    UE_LOG(LogTemp, Log, TEXT("EnemySpawner: Cleared all enemies"));
}