// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_Death.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UGA_Death : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;
private:
	UGA_Death();

	UFUNCTION()
	void OnDeathMontageCompleted();

	UFUNCTION()
	void OnDeathMontageCancelled();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Death")
	TSubclassOf<class UAnimMontage> DeathMontage;
};
