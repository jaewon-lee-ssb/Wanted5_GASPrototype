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

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles look inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoLook(float Yaw, float Pitch);

	UFUNCTION()
	void JumpPressed();

	UFUNCTION()
	void JumpReleased();

	UFUNCTION()
	void EnterAimMode();

	UFUNCTION()
	void ExitAimMode();

	UFUNCTION()
	bool PerformAimTrace(FHitResult& OutHit);

	UFUNCTION()
	bool IsAimModeActive() const;

protected:

	virtual void PossessedBy(AController* NewController) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for looking input */
	void ToggleAimMode(const FInputActionValue& Value);

	void Fire(const FInputActionValue& Value);

private:
	// Inherited via IAbilitySystemInterface
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	FGameplayAbilitySpecHandle AimAbilityHandle;
	FGameplayAbilitySpecHandle FireAbilityHandle;

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

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "UI")
	TObjectPtr<UUserWidget> AimCrosshairWidget;

private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;

	

};
