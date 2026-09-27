// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpec.h"
#include "Abilities/GameplayAbility.h"
#include "GA_ToggleAimMode.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UGA_ToggleAimMode : public UGameplayAbility
{
	GENERATED_BODY()
	
private:
	UGA_ToggleAimMode(const FObjectInitializer& ObjectInitializer);

public:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	bool bEnteredAimMode = false;
};
