// Fill out your copyright notice in the Description page of Project Settings.


#include "AGASCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "AbilitySystemComponent.h"

#include "Abilities/GameplayAbility_CharacterJump.h"

// Sets default values
AAGASCharacter::AAGASCharacter()
{
	//
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);

	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.f;


	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);
	FollowCamera->bUsePawnControlRotation = false;

	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));

}

void AAGASCharacter::DoMove(float Right, float Forward)
{
	if (!FollowCamera)
	{
		return;
	}

	FVector ForwardDirection = FollowCamera->GetForwardVector();
	FVector RightDirection = FollowCamera->GetRightVector();

	ForwardDirection.Z = 0.f;
	RightDirection.Z = 0.f;
	ForwardDirection.Normalize();
	RightDirection.Normalize();

	GetCharacterMovement()->AddInputVector(ForwardDirection * Forward);
	GetCharacterMovement()->AddInputVector(RightDirection * Right);
}

void AAGASCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController())
	{
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AAGASCharacter::JumpPressed()
{
	ASC->PressInputID(1);
}

void AAGASCharacter::JumpReleased()
{
	ASC->ReleaseInputID(1);
}

void AAGASCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ASC->InitAbilityActorInfo(this, this);

	if (HasAuthority())
	{
		ASC->GiveAbility(FGameplayAbilitySpec(UGameplayAbility_CharacterJump::StaticClass(), 1, 1));
	}

}

// Called to bind functionality to input
void AAGASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAGASCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AAGASCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AAGASCharacter::JumpPressed);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AAGASCharacter::JumpReleased);


	}
	else
	{
		UE_LOG(LogTemp, Error, 
			TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system.If you intend to use the legacy system, then you will need to update this C++ file."),
			*GetNameSafe(this));
	}
}

void AAGASCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	
	DoMove(MovementVector.X, MovementVector.Y);
}

void AAGASCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

UAbilitySystemComponent* AAGASCharacter::GetAbilitySystemComponent() const
{
	return ASC;
}

