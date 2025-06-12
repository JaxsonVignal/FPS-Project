// ChasingEnemy.cpp
#include "ChasingEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/DamageEvents.h"
#include "Engine/Engine.h"
#include "Engine/World.h"

AChasingEnemy::AChasingEnemy()
{
    PrimaryActorTick.bCanEverTick = true;
    LastAttackTime = 0.0f;
}

void AChasingEnemy::BeginPlay()
{
    Super::BeginPlay();

    // Get a reference to the player
    PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    // Debug: Check if this enemy has a controller
    if (GetController())
    {
        UE_LOG(LogTemp, Warning, TEXT("ChasingEnemy has controller: %s"), *GetController()->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("ChasingEnemy missing controller!"));
    }
}

void AChasingEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (PlayerPawn)
    {
        // Get direction and distance
        FVector ToPlayer = PlayerPawn->GetActorLocation() - GetActorLocation();
        float Distance = ToPlayer.Size();

        // Ignore vertical difference for movement
        FVector ToPlayerFlat = ToPlayer;
        ToPlayerFlat.Z = 0;
        ToPlayerFlat.Normalize();

        // Rotate to face the player
        FRotator TargetRotation = ToPlayerFlat.Rotation();
        FRotator CurrentRotation = GetActorRotation();

        // Smooth rotation
        FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 5.0f);
        SetActorRotation(NewRotation);

        // Check if player is in attack range
        if (IsPlayerInAttackRange())
        {
            // Attack if cooldown is ready
            float CurrentTime = GetWorld()->GetTimeSeconds();
            if (CurrentTime - LastAttackTime >= AttackCooldown)
            {
                AttackPlayer();
                LastAttackTime = CurrentTime;
            }
            // Don't move when attacking
        }
        else
        {
            // Move toward player if not in attack range
            AddMovementInput(ToPlayerFlat, MovementSpeed * DeltaTime);
        }
    }
}

bool AChasingEnemy::IsPlayerInAttackRange() const
{
    if (!PlayerPawn)
    {
        return false;
    }

    float Distance = FVector::Dist(GetActorLocation(), PlayerPawn->GetActorLocation());
    return Distance <= AttackRange;
}

void AChasingEnemy::AttackPlayer()
{
    if (!PlayerPawn)
    {
        return;
    }

    // Create damage event
    FDamageEvent DamageEvent;

    // Deal damage to the player
    PlayerPawn->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
}