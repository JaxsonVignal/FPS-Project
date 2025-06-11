// EnemySpawnerNew.cpp
#include "EnemySpawnerNew.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemySpawnerNew::AEnemySpawnerNew()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AEnemySpawnerNew::BeginPlay()
{
    Super::BeginPlay();
    if (EnemyClass)
    {
        GetWorldTimerManager().SetTimer(SpawnTimer, this, &AEnemySpawnerNew::SpawnEnemy, SpawnInterval, true);
    }
}

void AEnemySpawnerNew::SpawnEnemy()
{
    if (EnemyClass && GetWorld())
    {
        FVector SpawnLocation = GetActorLocation() + FVector(
            FMath::RandRange(-200, 200),
            FMath::RandRange(-200, 200),
            0  // Keep original spawn height
        );

        // Use proper spawn parameters for Character spawning
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

        // Spawn as the specific class type, not AActor
        AActor* SpawnedEnemy = GetWorld()->SpawnActor<AActor>(EnemyClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);

        // Force the character to update its movement component if it's a character
        if (ACharacter* CharacterEnemy = Cast<ACharacter>(SpawnedEnemy))
        {
            if (UCharacterMovementComponent* MovementComp = CharacterEnemy->GetCharacterMovement())
            {
                MovementComp->SetMovementMode(MOVE_Walking);
                MovementComp->UpdateComponentVelocity();
            }
        }
    }
}