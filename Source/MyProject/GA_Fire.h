// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_Fire.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UGA_Fire : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGA_Fire();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TSubclassOf<class UGameplayEffect> DamageEffectClass;
};
