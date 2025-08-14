// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CWeaponStructures.h"
#include "Player/Components/CWeaponComponent.h"
#include "CEquipment.generated.h"

/**
 * 
 */
UCLASS()
class OPERRATION_DDT_API UCEquipment : public UObject
{
	GENERATED_BODY()
	

public:
	void BeginPlay(class ACharacter* InOwner, const FEquipmentData& InData);
	
	UFUNCTION(BlueprintNativeEvent)
	void Equip();

	UFUNCTION(BlueprintNativeEvent)
	void Begin_Equip();

	UFUNCTION(BlueprintNativeEvent)
	void End_Equip();

	UFUNCTION(BlueprintNativeEvent)
	void UnEquip();



private:
	class ACharacter* OwnerCharacter;
	FEquipmentData Data;

	class UCMovementComponent* Movement;
	class UCStateComponent* State;

	bool bBeginEquip;	
	bool bEquipped;		


};
