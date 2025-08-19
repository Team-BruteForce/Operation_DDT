// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CBossWeaponStuctures.h"
#include "UObject/NoExportTypes.h"
#include "CBossEquipment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE ( FBossEquipmentBeginEquip );
DECLARE_DYNAMIC_MULTICAST_DELEGATE ( FBossEquipmentUnequip );

UCLASS()
class OPERRATION_DDT_API UCBossEquipment : public UObject
{
	GENERATED_BODY()
public:
	void BeginPlay ( class ACharacter* InOwner , TArray<FBossEquipmentData>& );

public:
	UFUNCTION ( BlueprintNativeEvent )
	void Equip ();

	UFUNCTION ( BlueprintNativeEvent )
	void Begin_Equip ( );

	UFUNCTION ( BlueprintNativeEvent )
	void End_Equip();

	UFUNCTION ( BlueprintNativeEvent )
	void Unequip();
public:
	FBossEquipmentBeginEquip OnBossEquipmentBeginEquip;
	FBossEquipmentUnequip OnBossEquipmentUnequip;

private:
	class ACharacter* OwnerCharacter;
	TArray<FBossEquipmentData> Data;

private:
	class UCBossMovementComponent* Movement;
	class UBossStateComponent* State;

private:
	bool bBeginEquip;
	bool bEquipped;
};
