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

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipmentBeginEquip);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipmentUnEquip);

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


public:
	FEquipmentBeginEquip OnEquipmentBeginEquip;
	FEquipmentUnEquip OnEquipmentUnEquip;

	
private:
	class ACharacter* OwnerCharacter;
	FEquipmentData Data;

	class UCMovementComponent* Movement;
	class UCStateComponent* State;

	bool bBeginEquip;	
	bool bEquipped;		


};
