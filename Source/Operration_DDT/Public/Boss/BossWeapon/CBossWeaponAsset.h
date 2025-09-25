/**
 * @file CBossWeaponAsset.h
 * @brief 보스 웨폰 에셋 헤더 파일
 * 
 * @details
 * 보스의 웨폰 에셋을 정의하는 데이터 에셋입니다.
 * 웨폰의 액션과 장비 데이터를 관리합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#pragma once

#include "CoreMinimal.h"
#include "CBossWeaponStuctures.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "CBossWeaponAsset.generated.h"

/**
 * @brief 보스 웨폰 에셋
 * 
 * 보스의 웨폰 에셋을 정의하는 데이터 에셋입니다.
 * 웨폰의 액션과 장비 데이터를 관리합니다.
 */
UCLASS()
class OPERRATION_DDT_API UCBossWeaponAsset : public UDataAsset
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACBossWeapon> BossWeaponClass;

	UPROPERTY(EditAnywhere)
	TMap<FGameplayTag, FBossEquipmentData> BossEquipmentData;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCBossEquipment> BossEquipmentClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCBossDoAction> BossDoActionClass;

	UPROPERTY(EditAnywhere)
	TMap<FGameplayTag,FBossMeleeLightAttack> BossDoActionDatas;
	
	UPROPERTY(EditAnywhere)
	TMap<FGameplayTag,FBossHitData> HitDatas;
public:
	FORCEINLINE class ACBossWeapon* GetBossWeapon ( ) { return BossWeapon; }
	FORCEINLINE class UCBossEquipment* GetBossEquipment ( ) { return BossEquipment; }
	FORCEINLINE class UCBossDoAction* GetBossDoAction ( ) { return BossDoAction; }

public:
	UCBossWeaponAsset();

	void BeginPlay(class ACharacter* InOwner);

	UPROPERTY()
	class ACBossWeapon* BossWeapon;

	UPROPERTY()
	class UCBossEquipment* BossEquipment;

	UPROPERTY()
	class UCBossDoAction* BossDoAction;

};
