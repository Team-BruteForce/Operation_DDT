// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Components/CStateComponent.h"
#include "Components/CWeaponComponent.h"
#include "CPlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class OPERRATION_DDT_API UCPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category="Animation")
	float Speed;

	UPROPERTY(BlueprintReadOnly, Category="Animation")
	float Pitch;

	UPROPERTY(BlueprintReadOnly, Category="Animation")
	float Direction;

	UPROPERTY(BlueprintReadOnly, Category="Animation")
	bool bAimMode = false;

	UPROPERTY(BlueprintReadOnly, Category="Animation")
	bool bIsSprinting = false;

	UPROPERTY(BlueprintReadOnly, Category="Animation")
	float ForwardInput;

	UPROPERTY(BlueprintReadOnly, Category="Animation")
	float RightInput;

protected:
		UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Animation")
		EWeaponType WeaponType = EWeaponType::Max;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Animation")
	EStateType PlayerStateType = EStateType::Idle;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Animation")
	class ADDTPlayer* OwnerCharacter;

public:
	void NativeBeginPlay() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	
	class UCWeaponComponent* Weapon;

	FRotator PrevRotation;

	UFUNCTION()
	void OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType);

	
	
	
};
