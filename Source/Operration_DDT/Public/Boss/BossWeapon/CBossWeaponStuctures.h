/**
 * @file CBossWeaponStuctures.h
 * @brief 보스 웨폰 구조체 헤더 파일
 * 
 * @details
 * 보스의 웨폰 관련 구조체들을 정의하는 파일입니다.
 * 보스의 공격 패턴과 몽타주를 관리합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DamageEvents.h"
#include "CBossWeaponStuctures.generated.h"

// 공통 몽타주 실행 함수 (헬퍼 함수)
void PlayBossAttackMontage(class ACharacter* InOwner, class UAnimMontage* Montage, float PlayRate, bool bCanMove, bool bFixedCamera);

USTRUCT()
struct FBossEquipmentData {
	GENERATED_BODY ( )
public:
	UPROPERTY(EditAnywhere)
	class UAnimMontage* Montage;
	UPROPERTY(EditAnywhere)
	float PlayRate =1;
	UPROPERTY(EditAnywhere)
	bool bCanMove =true;
	UPROPERTY(EditAnywhere)
	bool bUseControlRotation=true;
};

USTRUCT()
struct FBossMeleeLightAttack {
	GENERATED_BODY ( )
public:
	UPROPERTY(EditAnywhere)
	class UAnimMontage* Montage;
	UPROPERTY(EditAnywhere)
	float Damage=1;
	UPROPERTY(EditAnywhere)
	float PlayRate =1;
	UPROPERTY(EditAnywhere)
	bool bCanMove =true;
	UPROPERTY(EditAnywhere)
	bool bFixedCamera;
	UPROPERTY(EditAnywhere)
	class UFXSystemAsset* Effect;
	UPROPERTY(EditAnywhere)
	FVector EffectLocation=FVector::ZeroVector;
	UPROPERTY(EditAnywhere)
	FVector EffectScale = FVector::OneVector;

public:
	void PlayPaseOneMontage(class ACharacter* InOwner);
};

USTRUCT()
struct FBossHitData {
	GENERATED_BODY ( )
public:
	UPROPERTY ( EditAnywhere )
	class UAnimMontage* Montage;
	UPROPERTY ( EditAnywhere )
	float PlayRate = 1;
	UPROPERTY ( EditAnywhere )
	float Power;
	UPROPERTY ( EditAnywhere )
	float Launch = 100;
	UPROPERTY(EditAnywhere)
	float StopTime;
	UPROPERTY ( EditAnywhere )
	class USoundWave* Sound;
	UPROPERTY ( EditAnywhere )
	class UFXSystemAsset* Effect;
	UPROPERTY(EditAnywhere)
	FVector EffectLocation=FVector::ZeroVector;
	UPROPERTY(EditAnywhere)
	FVector EffectScale = FVector::OneVector;

public:
	void SendDamage ( class ACharacter* InAttacker , AActor* InAttackCauser , class ACharacter* InOther );
	void PlayMontage ( class ACharacter* InOwner );
	void PlayHitStop ( UWorld* InWorld );
	void PlaySoundWave ( class ACharacter* InOwner );
	void PlayEffect ( UWorld* InWorld , const FVector& InLocation );
	void PlayEffect ( UWorld* InWorld , const FVector& InLocation , const FRotator& InRotation );
};
USTRUCT ( )
struct FActionDamageEvent:public FDamageEvent
{
	GENERATED_BODY ( )
public:
	FBossHitData* HitData;
};
UCLASS()
class OPERRATION_DDT_API UCBossWeaponStuctures : public UObject
{
	GENERATED_BODY()
};
