// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Fire.h"

#include "AGASCharacter.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/EngineTypes.h"

UGA_Fire::UGA_Fire()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_Fire::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	AAGASCharacter* Character = ActorInfo ? Cast<AAGASCharacter>(ActorInfo->AvatarActor.Get()) : nullptr;

	// 캐릭터가 유효하고 조준중일때 발사
	if (!IsValid(Character) || !Character->IsAimModeActive())
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	FHitResult Hit;

	if (Character->PerformAimTrace(Hit))
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s / Component: %s / Point: %s"),
			*GetNameSafe(Hit.GetActor()),
			*GetNameSafe(Hit.GetComponent()),
			*Hit.ImpactPoint.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Hit"));
	}

	// 한번의 발사는 완료 조준 어빌리티는 그대로 유지
	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
}
