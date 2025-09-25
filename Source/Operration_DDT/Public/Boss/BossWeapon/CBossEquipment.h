/**
 * @file CBossEquipment.h
 * @brief 보스 장비 헤더 파일
 * 
 * @details
 * 보스의 장비 시스템을 구현하는 클래스입니다.
 * 보스의 장비 데이터와 장비 관리 기능을 제공합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#pragma once

#include "CoreMinimal.h"
#include "CBossWeaponStuctures.h"
#include "GameplayTagContainer.h"
#include "CBossEquipment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE ( FBossEquipmentBeginEquip );
DECLARE_DYNAMIC_MULTICAST_DELEGATE ( FBossEquipmentUnequip );

/**
 * @brief 보스 장비
 * 
 * 보스의 장비 시스템을 구현하는 클래스입니다.
 * 보스의 장비 데이터와 장비 관리 기능을 제공합니다.
 */
UCLASS()
class OPERRATION_DDT_API UCBossEquipment : public UObject
{
	GENERATED_BODY()
public:
	void BeginPlay ( class ACharacter* InOwner , TMap<FGameplayTag,FBossEquipmentData>& );

public:
	UFUNCTION ( BlueprintNativeEvent )
	void Equip (FGameplayTag PlayTag);

	UFUNCTION ( BlueprintNativeEvent )
	void Begin_Equip ( );

	UFUNCTION ( BlueprintNativeEvent )
	void End_Equip();

	UFUNCTION ( BlueprintNativeEvent )
	void Unequip(FGameplayTag PlayTag);
public:
	FBossEquipmentBeginEquip OnBossEquipmentBeginEquip;
	FBossEquipmentUnequip OnBossEquipmentUnequip;

private:
	class ACharacter* OwnerCharacter;
	TMap<FGameplayTag,FBossEquipmentData> Data;

private:
	class UCBossMovementComponent* Movement;
	class UBossStateComponent* State;

private:
	bool bBeginEquip;
	bool bEquipped;
};
