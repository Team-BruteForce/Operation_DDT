// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CWeaponStructures.generated.h"

/**
 * 
 */

USTRUCT()
struct FEquipmentData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
	float PlayRate = 1.f;

	UPROPERTY(EditAnywhere)
	bool bCanMove = true;

	UPROPERTY(EditAnywhere)
	bool bUseControlRotation = true;
		

};

USTRUCT()
struct FDoActionData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
	float PlayRate = 1.f;
	
	UPROPERTY(EditAnywhere)
	bool bCanMove = true;

	UPROPERTY(EditAnywhere)
	bool bFixedCamera;

	UPROPERTY(EditAnywhere)
	class UFXSystemAsset* Effect;

	UPROPERTY(EditAnywhere)
	FVector EffectLocation = FVector::ZeroVector;	// Effect 지정 방향의 보정치

	UPROPERTY(EditAnywhere)
	FVector EffectScale = FVector::OneVector; // Effect 크기 기본값 1 설정

	UPROPERTY(EditAnywhere)
	class USoundCue* FireSound;

	UPROPERTY(EditAnywhere)
	class USoundCue* EmptySound;
	

public:
	void DoAction(class ACharacter* InOwner);
	
};


UCLASS()
class OPERRATION_DDT_API UCWeaponStructures : public UObject
{
	GENERATED_BODY()


	
};
