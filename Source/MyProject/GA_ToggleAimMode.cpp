// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_ToggleAimMode.h"
#include "AGASCharacter.h"

UGA_ToggleAimMode::UGA_ToggleAimMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_ToggleAimMode::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!ActorInfo)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	AAGASCharacter* Character = Cast<AAGASCharacter>(ActorInfo->AvatarActor.Get());

	if (!IsValid(Character) || !HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	bEnteredAimMode = true;
	Character->EnterAimMode();
}

void UGA_ToggleAimMode::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (!IsEndAbilityValid(Handle, ActorInfo))
	{
		return;
	}

	if (ScopeLockCount > 0)
	{
		WaitingToExecute.Add(FPostLockDelegate::CreateUObject(this, &UGA_ToggleAimMode::EndAbility, Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled));
		return;
	}

	if (bEnteredAimMode)
	{
		bEnteredAimMode = false;

		AAGASCharacter* Character = ActorInfo ? Cast<AAGASCharacter>(ActorInfo->AvatarActor.Get()) : nullptr;

		if (IsValid(Character))
		{
			Character->ExitAimMode();
		}
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
