// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputAction.h"
#include "FPSCharacter.generated.h"

class UInputMappingContext;
class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class AFPSProjectile;
class USoundBase;
class UAnimSequence;
class UParticleSystem;

UCLASS()
class AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	// -- Enhanced Input -- //
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* DefaultInputMapping;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* Input_Move;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* Input_Look;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* Input_Jump;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* Input_Fire;

	/** Pawn mesh: 1st person view  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* Mesh1PComponent;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* GunMeshComponent;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	TSubclassOf<UCameraShakeBase> LandedCameraShake;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	TSubclassOf<UCameraShakeBase> JumpCameraShake;

	// -- Health System -- //
	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	float CurrentHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	bool bIsDead;

public:
	AFPSCharacter();

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<AFPSProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	UAnimSequence* FireAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	UParticleSystem* MuzzleFlash;

	// Override TakeDamage
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void Landed(const FHitResult& Hit) override;
	virtual void OnJumped_Implementation() override;

	// Health functions
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealthPercentage() const;

	UFUNCTION(BlueprintPure, Category = "Health")
	bool IsDead() const { return bIsDead; }

protected:
	void Fire();
	void MoveInput(const FInputActionValue& InputValue);
	void LookInput(const FInputActionValue& InputValue);
	void Die();

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1PComponent; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return CameraComponent; }
};