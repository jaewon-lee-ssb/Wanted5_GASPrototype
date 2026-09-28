// Fill out your copyright notice in the Description page of Project Settings.


#include "AGASCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Engine/EngineTypes.h"
#include "CollisionQueryParams.h"
#include "DrawDebugHelpers.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "AbilitySystemComponent.h"

#include "Blueprint/UserWidget.h"

#include "Abilities/GameplayAbility_CharacterJump.h"
#include "GA_ToggleAimMode.h"
#include "GA_Fire.h"

#include "GASCharacterAttributeSet.h"

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

	CharacterAttributeSet = CreateDefaultSubobject<UGASCharacterAttributeSet>(TEXT("CharacterAttributeSet"));
	ASC->AddAttributeSetSubobject<UGASCharacterAttributeSet>(CharacterAttributeSet);

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

void AAGASCharacter::EnterAimMode()
{
	if (AController* CharacterController = GetController())
	{
		CharacterController->SetIgnoreMoveInput(true);

		if (IsValid(AimCrosshairWidget))
		{
			AimCrosshairWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	}
}

void AAGASCharacter::ExitAimMode()
{
	if (AController* CharacterController = GetController())
	{
		CharacterController->SetIgnoreMoveInput(false);

		if (IsValid(AimCrosshairWidget))
		{
			AimCrosshairWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

bool AAGASCharacter::PerformAimTrace(FHitResult& OutHit)
{
	OutHit = FHitResult();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	UWorld* World = GetWorld();

	if (!PlayerController || !World)
	{
		return false;
	}

	FVector ViewLocation;
	FRotator ViewRotation;

	PlayerController->GetPlayerViewPoint(ViewLocation, ViewRotation);

	const float TraceDistance = 10000.f;

	const FVector TraceStart = ViewLocation;
	const FVector TraceEnd = TraceStart + ViewRotation.Vector() * TraceDistance;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	const bool bHit = World->LineTraceSingleByChannel(
		OutHit,
		TraceStart,
		TraceEnd,
		ECC_Visibility,
		QueryParams
	);

	const FVector DebugEnd = bHit ? OutHit.ImpactPoint : TraceEnd;

	DrawDebugLine(
		World,
		TraceStart,
		TraceEnd,
		bHit ? FColor::Green : FColor::Red,
		false,
		2.f,
		0,
		1.f
	);

	if (bHit)
	{
		DrawDebugSphere(
			World,
			OutHit.ImpactPoint,
			8.f,
			12,
			FColor::Yellow,
			false,
			2.f
		);
	}


	return bHit;
}

bool AAGASCharacter::IsAimModeActive() const
{
	if (!ASC)
	{
		return false;
	}

	const FGameplayAbilitySpec* AimSpec = ASC->FindAbilitySpecFromHandle(AimAbilityHandle);

	return AimSpec && AimSpec->IsActive();
}

void AAGASCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ASC->InitAbilityActorInfo(this, this);

	if (HasAuthority())
	{
		ASC->GiveAbility(FGameplayAbilitySpec(UGameplayAbility_CharacterJump::StaticClass(), 1, 1));
		AimAbilityHandle = ASC->GiveAbility(FGameplayAbilitySpec(UGA_ToggleAimMode::StaticClass(), 1, 2));
		FireAbilityHandle = ASC->GiveAbility(FGameplayAbilitySpec(UGA_Fire::StaticClass(), 1, 3));
	}

}

void AAGASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	GASPostInitializeComponents();

	ASC->SetNumericAttributeBase(CharacterAttributeSet->GetMaxHealthAttribute(), 200.f);
	ASC->SetNumericAttributeBase(CharacterAttributeSet->GetHealthAttribute(), 200.f);
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

		EnhancedInputComponent->BindAction(ToggleAction, ETriggerEvent::Started, this, &AAGASCharacter::ToggleAimMode);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &AAGASCharacter::Fire);


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

void AAGASCharacter::ToggleAimMode(const FInputActionValue& Value)
{
	if (!ASC)
	{
		return;
	}

	FGameplayAbilitySpec* AimSpec = ASC->FindAbilitySpecFromHandle(AimAbilityHandle);

	if (!AimSpec)
	{
		return;
	}

	if (AimSpec->IsActive())
	{
		// 조준 중이면 취소 → EndAbility() → ExitAimMode()
		ASC->CancelAbilityHandle(AimAbilityHandle);
	}
	else
	{
		// 조준 중이 아니면 활성화 요청
		ASC->TryActivateAbility(AimAbilityHandle);
	}
}

void AAGASCharacter::Fire(const FInputActionValue& Value)
{
	if (ASC)
	{
		ASC->TryActivateAbility(FireAbilityHandle);
	}
}

UAbilitySystemComponent* AAGASCharacter::GetAbilitySystemComponent() const
{
	return ASC;
}

