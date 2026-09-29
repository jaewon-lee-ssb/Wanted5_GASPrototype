// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Death.h"

#include <Animation/AnimMontage.h>
#include <Abilities/Tasks/AbilityTask_PlayMontageAndWait.h>
#include "AGASCharacter.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "Kismet/KismetSystemLibrary.h"
#include <Kismet/KismetSystemLibrary.h>

void UGA_Death::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!DeathMontage)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	//UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
	// UGameplayAbility * OwningAbility,
	// FName TaskInstanceName, 
	// UAnimMontage * MontageToPlay, 
	// float Rate, 
	// FName StartSection,
	// bool bStopWhenAbilityEnds,
	// float AnimRootMotionTranslationScale, 
	// float StartTimeSeconds, 
	// bool bAllowInterruptAfterBlendOut)

	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,								// GA_Death
		NAME_None,							// Task 식별 이름
		DeathMontage.GetDefaultObject(),	// 재생할 몽타주
		1.f,								// 재생 속도
		NAME_None,							// 지정 섹션 없이 시작
		true,								// 종료시 재생 중단
		1.f,								// 루트모션 이동 배율
		0.f,								// 시작 시간
		false								// 블렌드 아웃 중 중단도 알림
	);

	ActorInfo->AvatarActor.Get()->SetActorEnableCollision(false);

	if (AAGASCharacter* Character = Cast<AAGASCharacter>(ActorInfo->AvatarActor.Get()))
	{
		Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	}
	

	
	Task->OnCompleted.AddDynamic(this, &UGA_Death::OnDeathMontageCompleted);

	Task->OnInterrupted.AddDynamic(this, &UGA_Death::OnDeathMontageCancelled);

	Task->OnCancelled.AddDynamic(this, &UGA_Death::OnDeathMontageCancelled);

	Task->ReadyForActivation();
}

UGA_Death::UGA_Death()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_Death::OnDeathMontageCompleted()
{
	if (!IsActive())
	{
		return;
	}
	
	

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_Death::OnDeathMontageCancelled()
{
	if (!IsActive())
	{
		return;
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
