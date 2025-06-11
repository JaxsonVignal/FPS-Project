
// EnemySpawnerNew.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/TimerHandle.h"
#include "EnemySpawnerNew.generated.h"

UCLASS(BlueprintType, Blueprintable)
class FPSGAME_API AEnemySpawnerNew : public AActor
{
    GENERATED_BODY()

public:
    AEnemySpawnerNew();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
    TSubclassOf<AActor> EnemyClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
    float SpawnInterval = 2.0f;

private:
    FTimerHandle SpawnTimer;

    UFUNCTION()
    void SpawnEnemy();
};