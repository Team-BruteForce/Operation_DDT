// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "BossEffectComponent.generated.h"

/**
 * @brief 보스 이펙트 컴포넌트
 * 
 * @details
 * 보스 캐릭터에 붙어서 이펙트 시스템을 관리하는 컴포넌트입니다.
 * 이펙트 재생 요청을 받아서 적절한 이펙트를 실행합니다.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class OPERRATION_DDT_API UBossEffectComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBossEffectComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Data")
	class UDataTable* EffectDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Pool")
	int32 MaxPoolSize = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Pool")
	bool bAutoExpandPool = true;

	UPROPERTY(BlueprintReadOnly, Category = "Effect")
	class UBossEffectManager* EffectManager;

	UPROPERTY(BlueprintReadOnly, Category = "Effect")
	class UBossEffectExecute* EffectExecute;

	UPROPERTY(BlueprintReadOnly, Category = "Effect")
	class ACharacter* OwnerCharacter;

public:
	// ========================================
	// 기본 이펙트 실행 함수들
	// ========================================
	
	/**
	 * @brief 기본 이펙트 실행
	 * @details 보스 위치 또는 지정된 위치에서 이펙트를 한 번 실행
	 * @usage 일반적인 공격 이펙트, 폭발 이펙트 등
	 * 
	 * @param EffectTag 이펙트 태그
	 * @param InLocation 위치 (기본값: ZeroVector)
	 * @param InRotation 회전 (기본값: ZeroRotator)
	 * @param InScale 스케일 (기본값: 1,1,1)
	 */
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void PlayEffect(FGameplayTag EffectTag, const FVector& InLocation = FVector::ZeroVector, const FRotator& InRotation = FRotator::ZeroRotator, const FVector& InScale = FVector(1,1,1));

	/**
	 * @brief 여러 이펙트 동시 실행
	 * @details 여러 이펙트를 같은 위치에서 동시에 실행
	 * @usage 복합 공격 이펙트 (불+연기+폭발 등)
	 * 
	 * @param EffectTags 이펙트 태그 배열
	 * @param InLocation 위치 (기본값: ZeroVector)
	 * @param InRotation 회전 (기본값: ZeroRotator)
	 * @param InScale 스케일 (기본값: 1,1,1)
	 */
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void PlayEffects(TArray<FGameplayTag> EffectTags, const FVector& InLocation = FVector::ZeroVector, const FRotator& InRotation = FRotator::ZeroRotator, const FVector& InScale = FVector(1,1,1));

	// ========================================
	// 시간 관련 이펙트 실행 함수들
	// ========================================
	
	/**
	 * @brief 지연된 이펙트 실행
	 * @details 지정된 시간 후에 이펙트를 실행
	 * @usage 지연 폭발, 타이머 이펙트 등
	 * 
	 * @param EffectTag 이펙트 태그
	 * @param Delay 지연 시간 (초)
	 * @param InLocation 위치 (기본값: ZeroVector)
	 * @param InRotation 회전 (기본값: ZeroRotator)
	 * @param InScale 스케일 (기본값: 1,1,1)
	 */
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void PlayEffectWithDelay(FGameplayTag EffectTag, float Delay, const FVector& InLocation = FVector::ZeroVector, const FRotator& InRotation = FRotator::ZeroRotator, const FVector& InScale = FVector(1,1,1));

	/**
	 * @brief 루프 이펙트 실행
	 * @details 지정된 간격으로 이펙트를 반복 실행
	 * @usage 지속적인 오라 이펙트, 펄스 이펙트 등
	 * 
	 * @param EffectTag 이펙트 태그
	 * @param Interval 루프 간격 (초)
	 * @param LoopCount 루프 횟수 (0이면 무한)
	 * @param InLocation 위치 (기본값: ZeroVector)
	 * @param InRotation 회전 (기본값: ZeroRotator)
	 * @param InScale 스케일 (기본값: 1,1,1)
	 */
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void PlayEffectLoop(FGameplayTag EffectTag, float Interval, int32 LoopCount = 0, const FVector& InLocation = FVector::ZeroVector, const FRotator& InRotation = FRotator::ZeroRotator, const FVector& InScale = FVector(1,1,1));

	// ========================================
	// 소켓 관련 이펙트 실행 함수들
	// ========================================
	
	/**
	 * @brief 소켓 위치에서 이펙트 실행
	 * @details 보스의 특정 소켓 위치에서 이펙트를 실행 (소켓에 붙이지 않음)
	 * @usage 손에서 발사하는 투사체 이펙트, 발에서 나는 먼지 이펙트 등
	 * 
	 * @param EffectTag 이펙트 태그
	 * @param SocketName 소켓 이름 (예: "HandSocket", "FootSocket")
	 * @param InLocation 커스텀 위치 오프셋 (기본값: 소켓 위치)
	 * @param InRotation 회전 (기본값: 소켓 회전)
	 * @param InScale 스케일 (기본값: 1,1,1)
	 */
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void PlayEffectAtSocket(FGameplayTag EffectTag, const FName& SocketName, const FVector& InLocation = FVector::ZeroVector, const FRotator& InRotation = FRotator::ZeroRotator, const FVector& InScale = FVector(1,1,1));

	/**
	 * @brief 소켓에 붙여서 이펙트 실행
	 * @details 보스의 특정 소켓에 이펙트를 붙여서 실행 (보스가 움직여도 따라감)
	 * @usage 손에 붙는 불꽃, 머리에 붙는 오라, 무기에 붙는 효과 등
	 * 
	 * @param EffectTag 이펙트 태그
	 * @param SocketName 소켓 이름 (예: "HandSocket", "HeadSocket")
	 * @param InLocation 커스텀 위치 오프셋 (기본값: 소켓 위치)
	 * @param InRotation 회전 (기본값: 소켓 회전)
	 * @param InScale 스케일 (기본값: 1,1,1)
	 */
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void PlayEffectAttachedToSocket(FGameplayTag EffectTag, const FName& SocketName, const FVector& InLocation = FVector::ZeroVector, const FRotator& InRotation = FRotator::ZeroRotator, const FVector& InScale = FVector(1,1,1));

	/**
	 * @brief 소켓 위치에서 지연된 이펙트 실행
	 * @details 소켓 위치에서 지정된 시간 후에 이펙트를 실행
	 * @usage 손에서 지연 발사하는 마법, 발에서 지연 폭발하는 이펙트 등
	 * 
	 * @param EffectTag 이펙트 태그
	 * @param SocketName 소켓 이름
	 * @param Delay 지연 시간 (초)
	 * @param InLocation 커스텀 위치 오프셋 (기본값: 소켓 위치)
	 * @param InRotation 회전 (기본값: 소켓 회전)
	 * @param InScale 스케일 (기본값: 1,1,1)
	 */
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void PlayEffectAtSocketWithDelay(FGameplayTag EffectTag, const FName& SocketName, float Delay, const FVector& InLocation = FVector::ZeroVector, const FRotator& InRotation = FRotator::ZeroRotator, const FVector& InScale = FVector(1,1,1));

	/**
	 * @brief 소켓 위치에서 루프 이펙트 실행
	 * @details 소켓 위치에서 지정된 간격으로 이펙트를 반복 실행
	 * @usage 손에서 지속적으로 나는 불꽃, 머리에서 펄스하는 오라 등
	 * 
	 * @param EffectTag 이펙트 태그
	 * @param SocketName 소켓 이름
	 * @param Interval 루프 간격 (초)
	 * @param LoopCount 루프 횟수 (0이면 무한)
	 * @param InLocation 커스텀 위치 오프셋 (기본값: 소켓 위치)
	 * @param InRotation 회전 (기본값: 소켓 회전)
	 * @param InScale 스케일 (기본값: 1,1,1)
	 */
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void PlayEffectAtSocketLoop(FGameplayTag EffectTag, const FName& SocketName, float Interval, int32 LoopCount = 0, const FVector& InLocation = FVector::ZeroVector, const FRotator& InRotation = FRotator::ZeroRotator, const FVector& InScale = FVector(1,1,1));

	/**
	 * @brief 소켓에 붙여서 루프 이펙트 실행
	 * @details 소켓에 이펙트를 붙여서 지정된 간격으로 반복 실행
	 * @usage 지속적인 오라 이펙트, 연속 공격 이펙트 등
	 * 
	 * @param EffectTag 이펙트 태그
	 * @param SocketName 소켓 이름
	 * @param Interval 반복 간격 (초)
	 * @param LoopCount 반복 횟수 (0이면 무한)
	 * @param InLocation 위치 오프셋 (기본값: ZeroVector)
	 * @param InRotation 회전 오프셋 (기본값: ZeroRotator)
	 * @param InScale 스케일 (기본값: 1,1,1)
	 */
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void PlayEffectAttachedToSocketLoop(FGameplayTag EffectTag, const FName& SocketName, float Interval, int32 LoopCount = 0, const FVector& InLocation = FVector::ZeroVector, const FRotator& InRotation = FRotator::ZeroRotator, const FVector& InScale = FVector(1,1,1));
	
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
	 * @brief 보스 위치에서 이펙트 실행
	 * 
	 * @param EffectTag 이펙트 태그
	 */
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void PlayEffectAtBossLocation(FGameplayTag EffectTag);

	/**
	 * @brief 풀 크기 설정
	 * 
	 * @param NewPoolSize 새로운 풀 크기
	 */
	UFUNCTION(BlueprintCallable, Category = "Effect Pool")
	void SetPoolSize(int32 NewPoolSize);

	/**
	 * @brief 자동 풀 확장 설정
	 * 
	 * @param bAutoExpand 자동 확장 여부
	 */
	UFUNCTION(BlueprintCallable, Category = "Effect Pool")
	void SetAutoExpandPool(bool bAutoExpand);

	/**
	 * @brief 현재 활성화된 이펙트 수 반환
	 * 
	 * @return 활성화된 이펙트 수
	 */
	UFUNCTION(BlueprintPure, Category = "Effect")
	int32 GetActiveEffectCount() const;

	/**
	 * @brief 풀의 사용 가능한 이펙트 수 반환
	 * 
	 * @return 사용 가능한 이펙트 수
	 */
	UFUNCTION(BlueprintPure, Category = "Effect")
	int32 GetAvailableEffectCount() const;

protected:
	/**
	 * @brief 이펙트 매니저 초기화
	 */
	void InitializeEffectManager();

	/**
	 * @brief 위치가 기본값인지 확인하고 보스 위치로 설정
	 * 
	 * @param InLocation 입력 위치
	 * @return 실제 사용할 위치
	 */
	FVector GetEffectLocation(const FVector& InLocation);

	/**
	 * @brief 회전이 기본값인지 확인하고 보스 회전으로 설정
	 * 
	 * @param InRotation 입력 회전
	 * @return 실제 사용할 회전
	 */
	FRotator GetEffectRotation(const FRotator& InRotation);
};