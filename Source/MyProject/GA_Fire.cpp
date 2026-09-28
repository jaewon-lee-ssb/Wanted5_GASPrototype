// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Fire.h"

#include "AGASCharacter.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/EngineTypes.h"
#include <AbilitySystemBlueprintLibrary.h>
#include <GameplayEffect.h>

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
		//ApplyGameplayEffectToTarget(
		// const FGameplayAbilitySpecHandle Handle, 
		// const FGameplayAbilityActorInfo* ActorInfo, 
		// const FGameplayAbilityActivationInfo ActivationInfo, 
		// const FGameplayAbilityTargetDataHandle& Target, 
		// TSubclassOf<UGameplayEffect> GameplayEffectClass, 
		// float GameplayEffectLevel, 
		// int32 Stacks) const

		if (DamageEffectClass && IsValid(Hit.GetActor()))
		{
			const FGameplayAbilityTargetDataHandle TargetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromHitResult(Hit);
			ApplyGameplayEffectToTarget(
				Handle,					// 현재 발사 어빌리티
				ActorInfo,				// 공격자 정보
				ActivationInfo,			// 현재 실행 정보
				TargetData,				// 레이에 맞은 대상
				DamageEffectClass,		// 적용할 GE
				1.f,					// GE 레벨
				1						// 스택 수
			);		
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No Hit"));
		}
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Hit"));
	}

	// 한번의 발사는 완료 조준 어빌리티는 그대로 유지
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
