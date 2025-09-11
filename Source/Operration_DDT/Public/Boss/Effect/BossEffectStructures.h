// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "NiagaraSystem.h"
#include "BossEffectStructures.generated.h"

/**
 * @brief 보스 이펙트 데이터 구조체
 * 
 * @details
 * 데이터 테이블에서 사용되는 보스 이펙트 정보를 담는 구조체입니다.
 * Niagara System, 위치, 스케일, 지속시간 등의 정보를 포함합니다.
 */
USTRUCT(BlueprintType)
struct FBossEffectData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	FGameplayTag EffectTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	class UNiagaraSystem* EffectSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	FVector EffectLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	FRotator EffectRotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	FVector EffectScale = FVector::OneVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	float Duration = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	bool bAttachToBoss = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	bool bAutoDestroy = true;

	/**
	 * @brief 이펙트 재생
	 * 
	 * @param InWorld 월드 참조
	 * @param InLocation 위치
	 * @param InRotation 회전
	 */
	void PlayEffect(UWorld* InWorld, const FVector& InLocation, const FRotator& InRotation);

	/**
	 * @brief 이펙트 재생 (회전 없음)
	 * 
	 * @param InWorld 월드 참조
	 * @param InLocation 위치
	 */
	void PlayEffect(UWorld* InWorld, const FVector& InLocation);
};