// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "FPSProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimSequence.h"
#include "GameFramework/CharacterMovementComponent.h"

AFPSCharacter::AFPSCharacter()
{
	// Set default health values
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;
	bIsDead = false;

	// Create a CameraComponent	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	CameraComponent->SetupAttachment(GetCapsuleComponent());
	CameraComponent->SetRelativeLocation(FVector(0, 0, BaseEyeHeight));
	CameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view
	Mesh1PComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
	Mesh1PComponent->SetupAttachment(CameraComponent);
	Mesh1PComponent->CastShadow = false;
	Mesh1PComponent->SetRelativeRotation(FRotator(2.0f, -15.0f, 5.0f));
	Mesh1PComponent->SetRelativeLocation(FVector(0, 0, -160.0f));

	// Create a gun mesh component
	GunMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	GunMeshComponent->CastShadow = false;
	GunMeshComponent->SetupAttachment(Mesh1PComponent, "GripPoint");
}

void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(Input_Move, ETriggerEvent::Triggered, this, &AFPSCharacter::MoveInput);
	EnhancedInputComponent->BindAction(Input_Look, ETriggerEvent::Triggered, this, &AFPSCharacter::LookInput);
	EnhancedInputComponent->BindAction(Input_Jump, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(Input_Fire, ETriggerEvent::Triggered, this, &AFPSCharacter::Fire);

	const APlayerController* PC = GetController<APlayerController>();
	const ULocalPlayer* LP = PC->GetLocalPlayer();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	Subsystem->AddMappingContext(DefaultInputMapping, 0);
}

void AFPSCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	if (IsLocallyControlled())
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		if (PC && PC->PlayerCameraManager && LandedCameraShake)
		{
			PC->PlayerCameraManager->StartCameraShake(LandedCameraShake);
		}
	}
}

void AFPSCharacter::OnJumped_Implementation()
{
	Super::OnJumped_Implementation();

	if (IsLocallyControlled())
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		if (PC && PC->PlayerCameraManager && JumpCameraShake)
		{
			PC->PlayerCameraManager->StartCameraShake(JumpCameraShake);
		}
	}
}

void AFPSCharacter::Fire()
{
	if (bIsDead)
	{
		return;
	}

	if (ProjectileClass)
	{
		FVector MuzzleLocation = GunMeshComponent->GetSocketLocation("Muzzle");
		FRotator MuzzleRotation = GetControlRotation();

		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		ActorSpawnParams.Instigator = this;

		GetWorld()->SpawnActor<AFPSProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, ActorSpawnParams);
	}

	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	UAnimInstance* AnimInstance = Mesh1PComponent->GetAnimInstance();
	if (AnimInstance && FireAnimation)
	{
		AnimInstance->PlaySlotAnimationAsDynamicMontage(FireAnimation, "Arms", 0.0f);
	}
}

void AFPSCharacter::MoveInput(const FInputActionValue& InputValue)
{
	if (bIsDead)
	{
		return;
	}

	FVector2d MoveValue = InputValue.Get<FVector2d>();

	AddMovementInput(GetActorForwardVector(), MoveValue.X);
	AddMovementInput(GetActorRightVector(), MoveValue.Y);
}

void AFPSCharacter::LookInput(const FInputActionValue& InputValue)
{
	if (bIsDead)
	{
		return;
	}

	FVector2d LookValue = InputValue.Get<FVector2d>();

	AddControllerYawInput(LookValue.X);
	AddControllerPitchInput(LookValue.Y);
}

float AFPSCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (bIsDead || DamageAmount <= 0.0f)
	{
		return 0.0f;
	}

	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	CurrentHealth = FMath::Clamp(CurrentHealth - ActualDamage, 0.0f, MaxHealth);

	if (CurrentHealth <= 0.0f)
	{
		Die();
	}

	return ActualDamage;
}

void AFPSCharacter::Die()
{
	if (bIsDead)
	{
		return;
	}

	bIsDead = true;
	CurrentHealth = 0.0f;

	// Disable collision
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Stop movement
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->StopMovementImmediately();

	// Restart the level immediately
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()));
}

float AFPSCharacter::GetHealthPercentage() const
{
	return MaxHealth > 0.0f ? CurrentHealth / MaxHealth : 0.0f;
}