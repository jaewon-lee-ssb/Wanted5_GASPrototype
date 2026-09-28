// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include <AbilitySystemComponent.h>
#include "GASCharacterAttributeSet.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAttributeDataChanged, float, OldValue, float, NewValue);

/**
 * 
 */
UCLASS()
class MYPROJECT_API UGASCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UGASCharacterAttributeSet();

	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, MaxHealth);
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, Health);

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	UPROPERTY(BlueprintAssignable, Category = "Attribute")
	mutable FAttributeDataChanged OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Attribute")
	mutable FAttributeDataChanged OnHealthChanged;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MaxHealth;

	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData Health;
	
};
