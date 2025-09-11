// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "BossEffectStructures.h"
#include "BossEffectManager.generated.h"

/**
 * @brief 보스 이펙트 매니저
 * 
 * @details
 * 데이터 테이블 관리와 오브젝트 풀링을 담당하는 클래스입니다.
 * 이펙트 액터들을 풀링하여 성능을 최적화하고,
 * 데이터 테이블에서 이펙트 정보를 로드합니다.
 */
UCLASS()
class OPERRATION_DDT_API UBossEffectManager : public UObject
{
	GENERATED_BODY()

public:
	UBossEffectManager();

protected:
	UPROPERTY()
	class ACharacter* OwnerCharacter;

	UPROPERTY()
	class UWorld* World;

	UPROPERTY()
	class UDataTable* EffectDataTable;

public:
	UPROPERTY()
	TMap<FGameplayTag, FBossEffectData> EffectDatas;

protected:

	// 오브젝트 풀
	UPROPERTY()
	TArray<class ABossEffect*> EffectPool;

	UPROPERTY()
	TArray<class ABossEffect*> ActiveEffects;

	UPROPERTY()
	int32 MaxPoolSize = 20;

	UPROPERTY()
	bool bAutoExpandPool = true;

	UPROPERTY()
	class UBossEffectExecute* EffectExecute;

public:
	/**
	 * @brief 게임 시작 시 호출되는 함수
	 * 
	 * @param InOwner 소유자 캐릭터
	 * @param InEffectDataTable 이펙트 데이터 테이블
	 * @param InMaxPoolSize 최대 풀 크기
	 * @param InAutoExpandPool 자동 풀 확장 여부
	 */
	void BeginPlay(ACharacter* InOwner, UDataTable* InEffectDataTable, int32 InMaxPoolSize = 20, bool InAutoExpandPool = true);

	/**
	 * @brief 이펙트 재생
	 * 
	 * @param EffectTag 이펙트 태그
	 * @param InLocation 위치
	 * @param InRotation 회전
	 * @param InScale 스케일
	 */
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void PlayEffect(FGameplayTag EffectTag, const FVector& InLocation, const FRotator& InRotation = FRotator::ZeroRotator, const FVector& InScale = FVector(1,1,1));

	/**
	 * @brief 소켓에 붙여서 이펙트 재생
	 * 
	 * @param EffectTag 이펙트 태그
	 * @param SocketName 소켓 이름
	 * @param InRotation 회전
	 * @param InScale 스케일
	 */
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void PlayEffectAttachedToSocket(FGameplayTag EffectTag, const FName& SocketName, const FVector& InLocation = FVector::ZeroVector, const FRotator& InRotation = FRotator::ZeroRotator, const FVector& InScale = FVector(1,1,1));

	/**
	 * @brief 모든 이펙트 정지
	 */
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void StopAllEffects();

	/**
	 * @brief 특정 태그의 이펙트 정지
	 * 
	 * @param EffectTag 이펙트 태그
	 */
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void StopEffect(FGameplayTag EffectTag);

	/**
	 * @brief 최대 풀 크기 설정
	 * 
	 * @param NewMaxPoolSize 새로운 최대 풀 크기
	 */
	UFUNCTION(BlueprintCallable, Category = "Effect Pool")
	void SetMaxPoolSize(int32 NewMaxPoolSize);

	/**
	 * @brief 자동 풀 확장 설정
	 * 
	 * @param bAutoExpand 자동 확장 여부
	 */
	UFUNCTION(BlueprintCallable, Category = "Effect Pool")
	void SetAutoExpandPool(bool bAutoExpand);

// ExecuteEffect 관련 함수들 제거 - BossEffectExecute에서 직접 처리

protected:
	/**
	 * @brief 데이터 테이블에서 데이터 로드
	 */
	void LoadDataFromTables();

	/**
	 * @brief 이펙트 풀 초기화
	 */
	void InitializeEffectPool();

	/**
	 * @brief 풀에서 이펙트 가져오기
	 * 
	 * @return 사용 가능한 이펙트 액터
	 */
	class ABossEffect* GetEffectFromPool();

	/**
	 * @brief 이펙트를 풀로 반환
	 * 
	 * @param InEffect 반환할 이펙트 액터
	 */
	void ReturnEffectToPool(class ABossEffect* InEffect);

	/**
	 * @brief 이펙트 풀 확장
	 */
	void ExpandEffectPool();

public:
	/**
	 * @brief 이펙트가 완료되었을 때 호출
	 * 
	 * @param InEffect 완료된 이펙트 액터
	 */
	UFUNCTION()
	void OnEffectFinished(class ABossEffect* InEffect);

	/**
	 * @brief 현재 활성화된 이펙트 수 반환
	 * 
	 * @return 활성화된 이펙트 수
	 */
	UFUNCTION(BlueprintPure, Category = "Effect")
	int32 GetActiveEffectCount() const { return ActiveEffects.Num(); }

	/**
	 * @brief 풀의 사용 가능한 이펙트 수 반환
	 * 
	 * @return 사용 가능한 이펙트 수
	 */
	UFUNCTION(BlueprintPure, Category = "Effect")
	int32 GetAvailableEffectCount() const { return EffectPool.Num(); }
};