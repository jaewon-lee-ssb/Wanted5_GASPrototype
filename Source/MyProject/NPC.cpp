// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"

#include "Components/CapsuleComponent.h"
#include "AbilitySystemComponent.h"

#include "GASCharacterAttributeSet.h"

// Sets default values
ANPC::ANPC()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));

	CharacterAttributeSet = CreateDefaultSubobject<UGASCharacterAttributeSet>(TEXT("CharacterAttributeSet"));
	ASC->AddAttributeSetSubobject<UGASCharacterAttributeSet>(CharacterAttributeSet);
}

void ANPC::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	GASPostInitializeComponents();

	ASC->SetNumericAttributeBase(CharacterAttributeSet->GetMaxHealthAttribute(), 200.f);
	ASC->SetNumericAttributeBase(CharacterAttributeSet->GetHealthAttribute(), 200.f);
}

UAbilitySystemComponent* ANPC::GetAbilitySystemComponent() const
{
	return ASC;
}

