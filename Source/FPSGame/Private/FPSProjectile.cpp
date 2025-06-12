
// FPSProjectile.cpp
#include "FPSProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "EnemyCharacter.h"
#include "ChasingEnemy.h"
#include "FPSGameMode.h"  // Added for score system

AFPSProjectile::AFPSProjectile()
{
    // Use a sphere as a simple collision representation
    CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    CollisionComp->InitSphereRadius(5.0f);
    CollisionComp->SetCollisionProfileName("Projectile");
    CollisionComp->OnComponentHit.AddDynamic(this, &AFPSProjectile::OnHit);	// set up a notification for when this component hits something blocking
    // Players can't walk on it
    CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
    CollisionComp->CanCharacterStepUpOn = ECB_No;
    // Set as root component
    RootComponent = CollisionComp;
    // Use a ProjectileMovementComponent to govern this projectile's movement
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
    ProjectileMovement->UpdatedComponent = CollisionComp;
    ProjectileMovement->InitialSpeed = 3000.f;
    ProjectileMovement->MaxSpeed = 3000.f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = true;
}

void AFPSProjectile::BeginPlay()
{
    Super::BeginPlay();
    FTimerHandle TimerHandle;
    GetWorldTimerManager().SetTimer(TimerHandle, this, &AFPSProjectile::Explode, 1.0f, false);
}

void AFPSProjectile::Explode()
{
    UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionFX, GetActorLocation(), FRotator::ZeroRotator, FVector(5.0f));
    // Allow BP to trigger additional logic
    BlueprintExplode();
    Destroy();
}

void AFPSProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor && OtherActor != this)
    {
        // Check if the actor is an enemy
        AChasingEnemy* Enemy = Cast<AChasingEnemy>(OtherActor);
        if (Enemy)
        {
            // Destroy the enemy
            Enemy->Destroy();

            // Update score when enemy is killed
            AFPSGameMode* GameMode = Cast<AFPSGameMode>(UGameplayStatics::GetGameMode(this));
            if (GameMode)
            {
                GameMode->EnemyKilled();
            }

            // Explode and destroy the projectile
            Explode();
            return;
        }
        // Original physics object behavior
        if (OtherComp && OtherComp->IsSimulatingPhysics())
        {
            float RandomIntensity = FMath::RandRange(200.0f, 500.0f);
            OtherComp->AddImpulseAtLocation(GetVelocity() * RandomIntensity, GetActorLocation());
            FVector Scale = OtherComp->GetComponentScale();
            Scale *= 0.8f;
            if (Scale.GetMin() < 0.5f)
            {
                OtherActor->Destroy();
            }
            else
            {
                OtherComp->SetWorldScale3D(Scale);
            }
            UMaterialInstanceDynamic* MatInst = OtherComp->CreateDynamicMaterialInstance(0);
            if (MatInst)
            {
                MatInst->SetVectorParameterValue("Color", FLinearColor::MakeRandomColor());
            }
            Explode();
        }
    }
}
