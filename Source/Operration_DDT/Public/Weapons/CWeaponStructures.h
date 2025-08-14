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


UCLASS()
class OPERRATION_DDT_API UCWeaponStructures : public UObject
{
	GENERATED_BODY()
	
};
