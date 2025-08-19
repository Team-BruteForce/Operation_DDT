// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CBossWeaponStuctures.h"
#include "Engine/DataAsset.h"
#include "CBossWeaponAsset.generated.h"

/**
 * 
 */
UCLASS()
class OPERRATION_DDT_API UCBossWeaponAsset : public UDataAsset
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACBossWeapon> BossWeaponClass;

	UPROPERTY(EditAnywhere)
	TArray< FBossEquipmentData> BossEquipmentData;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCBossEquipment> BossEquipmentClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCBossDoAction> BossDoActionClass;

	UPROPERTY(EditAnywhere)
	TArray<FBossDoActionData> BossDoActionDatas;
	
	UPROPERTY(EditAnywhere)
	TArray<FBossHitData> HitDatas;
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
