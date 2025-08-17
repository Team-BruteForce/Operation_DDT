// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
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

protected:
		UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Animation")
		EWeaponType WeaponType = EWeaponType::Max;

public:
	void NativeBeginPlay() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	class ACharacter* OwnerCharacter;
	class UCWeaponComponent* Weapon;

	FRotator PrevRotation;

	UFUNCTION()
	void OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType);
	
	
};
