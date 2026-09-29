// Fill out your copyright notice in the Description page of Project Settings.


#include "GASCharacterAttributeSet.h"

#include <GameplayEffectExtension.h>

UGASCharacterAttributeSet::UGASCharacterAttributeSet()
	: MaxHealth(100.f)
{
	InitHealth(GetMaxHealth());
}

void UGASCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	if (Attribute == GetMaxHealthAttribute())
	{
		NewValue = FMath::Max<float>(1.f, NewValue);
	}
	else if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
}

void UGASCharacterAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	if (Attribute == GetMaxHealthAttribute())
	{
		OnMaxHealthChanged.Broadcast(OldValue, NewValue);
	}
	else if (Attribute == GetHealthAttribute())
	{
		OnHealthChanged.Broadcast(OldValue, NewValue);
	}
}

void UGASCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	if (GetHealth() <= 0.f)
	{
		OnOutOfHealth.Broadcast(Data.EffectSpec.GetEffectContext().GetInstigator());
	}
}
