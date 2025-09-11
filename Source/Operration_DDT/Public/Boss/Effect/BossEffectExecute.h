// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BossEffectStructures.h"
#include "BossEffectExecute.generated.h"

/**
 * @brief 보스 이펙트 실행 클래스
 * 
 * @details
 * 보스의 이펙트를 실행하는 클래스입니다.
 * 이펙트 실행 로직을 담당하고 BossEffectManager와의 종속성을 분리합니다.
 */
UCLASS()
class OPERRATION_DDT_API UBossEffectExecute : public UObject
{
	GENERATED_BODY()

public:
	UBossEffectExecute();

protected:
	UPROPERTY()
	class ACharacter* OwnerCharacter;

	UPROPERTY()
	class UWorld* World;

	UPROPERTY()
	TMap<FGameplayTag, FBossEffectData> EffectDatas;

	UPROPERTY()
	class UBossEffectManager* EffectManager;

public:
	/**
	 * @brief 게임 시작 시 호출되는 함수
	 * 
	 * @param InOwner 소유자 캐릭터
	 * @param InEffectDatas 이펙트 데이터 맵
	 * @param InEffectManager 이펙트 매니저 참조
	 */
	void BeginPlay(ACharacter* InOwner, TMap<FGameplayTag, FBossEffectData>& InEffectDatas, class UBossEffectManager* InEffectManager);

	// ========================================
	// 기본 이펙트 실행 함수들 (내부용)
	// ========================================
	
	/**
	 * @brief 기본 이펙트 실행 (내부용)
	 * @details EffectManager를 통해 이펙트를 실행하는 내부 함수
	 */
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void ExecuteEffect(FGameplayTag EffectTag, const FVector& InLocation, const FRotator& InRotation = FRotator::ZeroRotator, const FVector& InScale = FVector(1,1,1));

	/**
	 * @brief 여러 이펙트 동시 실행 (내부용)
	 * @details 여러 이펙트를 같은 위치에서 동시에 실행하는 내부 함수
	 */
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void ExecuteEffects(TArray<FGameplayTag> EffectTags, const FVector& InLocation, const FRotator& InRotation = FRotator::ZeroRotator, const FVector& InScale = FVector(1,1,1));

	// ========================================
	// 시간 관련 이펙트 실행 함수들 (내부용)
	// ========================================
	
	/**
	 * @brief 지연된 이펙트 실행 (내부용)
	 * @details 지정된 시간 후에 이펙트를 실행하는 내부 함수
	 */
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void ExecuteEffectWithDelay(FGameplayTag EffectTag, const FVector& InLocation, const FRotator& InRotation, float Delay, const FVector& InScale = FVector(1,1,1));

	/**
	 * @brief 루프 이펙트 실행 (내부용)
	 * @details 지정된 간격으로 이펙트를 반복 실행하는 내부 함수
	 */
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void ExecuteEffectLoop(FGameplayTag EffectTag, const FVector& InLocation, const FRotator& InRotation, float Interval, int32 LoopCount = 0, const FVector& InScale = FVector(1,1,1));

	// ========================================
	// 소켓 관련 이펙트 실행 함수들 (내부용)
	// ========================================
	
	/**
	 * @brief 소켓 위치에서 이펙트 실행 (내부용)
	 * @details 보스의 특정 소켓 위치에서 이펙트를 실행하는 내부 함수
	 */
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void ExecuteEffectAtSocket(FGameplayTag EffectTag, const FName& SocketName, const FVector& InLocation = FVector::ZeroVector, const FRotator& InRotation = FRotator::ZeroRotator, const FVector& InScale = FVector(1,1,1));

	/**
	 * @brief 소켓에 붙여서 이펙트 실행 (내부용)
	 * @details 보스의 특정 소켓에 이펙트를 붙여서 실행하는 내부 함수
	 */
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void ExecuteEffectAttachedToSocket(FGameplayTag EffectTag, const FName& SocketName, const FVector& InLocation = FVector::ZeroVector, const FRotator& InRotation = FRotator::ZeroRotator, const FVector& InScale = FVector(1,1,1));

	/**
	 * @brief 소켓 위치에서 지연된 이펙트 실행 (내부용)
	 * @details 소켓 위치에서 지정된 시간 후에 이펙트를 실행하는 내부 함수
	 */
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void ExecuteEffectAtSocketWithDelay(FGameplayTag EffectTag, const FName& SocketName, float Delay, const FVector& InLocation = FVector::ZeroVector, const FRotator& InRotation = FRotator::ZeroRotator, const FVector& InScale = FVector(1,1,1));

	/**
	 * @brief 소켓 위치에서 루프 이펙트 실행 (내부용)
	 * @details 소켓 위치에서 지정된 간격으로 이펙트를 반복 실행하는 내부 함수
	 */
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void ExecuteEffectAtSocketLoop(FGameplayTag EffectTag, const FName& SocketName, float Interval, int32 LoopCount = 0, const FVector& InLocation = FVector::ZeroVector, const FRotator& InRotation = FRotator::ZeroRotator, const FVector& InScale = FVector(1,1,1));

	/**
	 * @brief 소켓에 붙여서 루프 이펙트 실행 (내부용)
	 * @details 소켓에 이펙트를 붙여서 지정된 간격으로 반복 실행하는 내부 함수
	 */
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void ExecuteEffectAttachedToSocketLoop(FGameplayTag EffectTag, const FName& SocketName, float Interval, int32 LoopCount = 0, const FVector& InLocation = FVector::ZeroVector, const FRotator& InRotation = FRotator::ZeroRotator, const FVector& InScale = FVector(1,1,1));

	/**
	 * @brief 이펙트 실행 시작
	 */
	void Begin_ExecuteEffect();

	/**
	 * @brief 이펙트 실행 종료
	 */
	void End_ExecuteEffect();
};
