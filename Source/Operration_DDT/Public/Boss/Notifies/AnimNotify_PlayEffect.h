// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "AnimNotify_PlayEffect.generated.h"

/**
 * @brief 이펙트 함수 타입
 */
UENUM(BlueprintType)
enum class EEffectFunctionType : uint8
{
	PlayEffect,                    // 기본 이펙트 실행
	PlayEffects,                   // 여러 이펙트 동시 실행
	PlayEffectWithDelay,           // 지연된 이펙트 실행
	PlayEffectLoop,                // 루프 이펙트 실행
	PlayEffectAtSocket,            // 소켓 위치에서 이펙트 실행
	PlayEffectAttachedToSocket,    // 소켓에 붙여서 이펙트 실행
	PlayEffectAtSocketWithDelay,   // 소켓 위치에서 지연된 이펙트 실행
	PlayEffectAtSocketLoop,        // 소켓 위치에서 루프 이펙트 실행
	PlayEffectAttachedToSocketLoop // 소켓에 붙여서 루프 이펙트 실행
};

/**
 * @brief 보스 이펙트 재생 애니메이션 노티파이
 * 
 * @details
 * 애니메이션 몽타주에서 보스 이펙트를 재생할 수 있는 노티파이입니다.
 * 다양한 이펙트 함수들을 선택하여 사용할 수 있습니다.
 */
UCLASS()
class OPERRATION_DDT_API UAnimNotify_PlayEffect : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAnimNotify_PlayEffect();

protected:
	// ========================================
	// 이펙트 함수 타입 선택
	// ========================================

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Settings")
	EEffectFunctionType EffectFunctionType = EEffectFunctionType::PlayEffect;

	// ========================================
	// 기본 설정
	// ========================================
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Settings")
	FGameplayTag EffectTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Settings")
	TArray<FGameplayTag> EffectTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Settings")
	FName SocketName = NAME_None;

	// ========================================
	// 위치 및 회전 설정
	// ========================================
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Settings")
	FVector CustomLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Settings")
	FRotator CustomRotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Settings")
	bool bUseCustomLocation = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Settings")
	bool bUseCustomRotation = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Settings")
	FVector CustomScale = FVector::OneVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Settings")
	bool bUseCustomScale = false;

	// ========================================
	// 시간 관련 설정
	// ========================================
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Settings", meta = (EditCondition = "EffectFunctionType == EEffectFunctionType::PlayEffectWithDelay || EffectFunctionType == EEffectFunctionType::PlayEffectAtSocketWithDelay"))
	float Delay = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Settings", meta = (EditCondition = "EffectFunctionType == EEffectFunctionType::PlayEffectLoop || EffectFunctionType == EEffectFunctionType::PlayEffectAtSocketLoop"))
	float LoopInterval = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Settings", meta = (EditCondition = "EffectFunctionType == EEffectFunctionType::PlayEffectLoop || EffectFunctionType == EEffectFunctionType::PlayEffectAtSocketLoop"))
	int32 LoopCount = 0;

public:
	FString GetNotifyName_Implementation() const override;

	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
