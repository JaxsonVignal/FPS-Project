// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ChasingEnemy.generated.h"

UCLASS()
class FPSGAME_API AChasingEnemy : public ACharacter
{
    GENERATED_BODY()

public:
    AChasingEnemy();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

private:
    APawn* PlayerPawn;

    UPROPERTY(EditAnywhere, Category = "Chase")
    float MovementSpeed = 50.0f;

    // Attack properties
    UPROPERTY(EditAnywhere, Category = "Attack")
    float AttackRange = 150.0f;

    UPROPERTY(EditAnywhere, Category = "Attack")
    float AttackDamage = 20.0f;

    UPROPERTY(EditAnywhere, Category = "Attack")
    float AttackCooldown = 1.0f;

    // Timer for attack cooldown
    float LastAttackTime;

    // Function to attack the player
    void AttackPlayer();

    // Check if player is in attack range
    bool IsPlayerInAttackRange() const;
};