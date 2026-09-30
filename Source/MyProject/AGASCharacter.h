// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayAbilitySpecHandle.h"
#include "AGASCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UUserWidget;
struct FInputActionValue;

UCLASS()
class MYPROJECT_API AAGASCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAGASCharacter();

	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	bool PerformAimTrace(FHitResult& OutHit);

	bool IsAimModeActive() const;

	void EnterAimMode();
	void ExitAimMode();

	UFUNCTION(BlueprintImplementableEvent)
	void GASPostInitializeComponents();

	UFUNCTION()
	void OnOutOfHealthCpp(AActor* InInstigator);

protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void PostInitializeComponents() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	void ToggleAimMode(const FInputActionValue& Value);

	void Fire(const FInputActionValue& Value);

private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	void DoMove(float Right, float Forward);
	void DoLook(float Yaw, float Pitch);

	void JumpPressed();
	void JumpReleased();

protected:

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	/** Toggle Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ToggleAction;

	/** Fire Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> FireAction;

	/** Run Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> RunAction;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "UI")
	TObjectPtr<UUserWidget> AimCrosshairWidget;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "UI")
	TObjectPtr<class UWidgetComponent> HpBar;

private:
	FGameplayAbilitySpecHandle AimAbilityHandle;
	FGameplayAbilitySpecHandle FireAbilityHandle;
	FGameplayAbilitySpecHandle RunAbilityHandle;
	FGameplayAbilitySpecHandle JumpAbilityHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TSubclassOf<class UGameplayAbility> FireAbilityClass;

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TSubclassOf<class UGameplayAbility> JumpAbilityClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAbilitySystemComponent> ASC;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UGASCharacterAttributeSet> CharacterAttributeSet;

};
