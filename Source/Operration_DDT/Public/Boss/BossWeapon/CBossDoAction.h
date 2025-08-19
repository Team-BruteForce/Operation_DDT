// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CBossWeaponStuctures.h"
#include "UObject/NoExportTypes.h"
#include "CBossDoAction.generated.h"

/**
 * 
 */
UCLASS()
class OPERRATION_DDT_API UCBossDoAction : public UObject
{
	GENERATED_BODY()
public:
	// UCBossDoAction();

	virtual void BeginPlay (
		class ACBossWeapon* InAttachment ,
		class UCBossEquipment* InEquipment ,
		class ACharacter* InOwner ,
		class TArray<FBossDoActionData>& InDoActionData,
		class TArray<FBossHitData>& InHitData
	);

public:
	virtual void DoAction ();
	virtual void Begin_DoAction ( );
	virtual void End_DoAction ( );

public:
	UFUNCTION()
		virtual void OnAttachmentBeginCollision() { }

	UFUNCTION()
		virtual void OnAttachmentEndCollision() { }

	UFUNCTION()
		virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCuaser, class ACharacter* InOther) { }

	UFUNCTION()
		virtual void OnAttachmentEndOverlap(class ACharacter* InAttacker, class ACharacter* InOther) { }

protected:
	bool bBeginAction;

	class ACharacter* OwnerCharacter;
	class UWorld* World;

	class UCBossMovementComponent* Movement;
	class UBossStateComponent* State;

	TArray<FBossDoActionData> BossDoActionDatas;
	TArray<FBossHitData> BossHitDatas;
};
