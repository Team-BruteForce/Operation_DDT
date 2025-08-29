// Fill out your copyright notice in the Description page of Project Settings.

/**
 * @brief 보스 타겟팅 컴포넌트 클래스
 * 
 * 보스 캐릭터의 타겟팅 로직을 관리하는 컴포넌트입니다.
 * 플레이어 위치 예측, 이동 방향 분석, 거리 상태 평가 등을 처리합니다.
 */

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "Boss/Data/BossTagStructure.h"
#include "CBossTargetingComponent.generated.h"

/**
 * @brief 보스 타겟팅 컴포넌트 클래스
 * 
 * 보스 캐릭터의 타겟팅 로직을 관리하는 컴포넌트입니다.
 * 플레이어 위치 예측, 이동 방향 분석, 거리 상태 평가 등을 처리합니다.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class OPERRATION_DDT_API UCBossTargetingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	/**
	 * @brief 생성자
	 */
	UCBossTargetingComponent();

protected:
	/**
	 * @brief 게임 시작 시 호출되는 함수
	 */
	virtual void BeginPlay() override;

public:	
	/**
	 * @brief 매 프레임 호출되는 틱 함수
	 * 
	 * @param DeltaTime 프레임 간 시간 간격
	 * @param TickType 틱 타입
	 * @param ThisTickFunction 틱 함수 정보
	 */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// ===== 타겟 위치 예측 함수들 =====
	
	/**
	 * @brief 플레이어의 속도와 가속도를 기반으로 미래 위치를 예측합니다.
	 * 
	 * @param OutTargetLocation 예측된 타겟 위치
	 * @param OutTargetRotation 예측된 타겟 회전
	 * @param DeltaTime 델타 타임
	 */
	UFUNCTION(BlueprintCallable, Category="Target Prediction")
	void CalculatePredictedTargetLocation(FVector& OutTargetLocation, FRotator& OutTargetRotation, const float DeltaTime);

	/**
	 * @brief 4가지 예측 방법을 가중 평균으로 조합하여 최종 예측 위치를 계산합니다.
	 * 
	 * @param PlayerVelocity 플레이어 속도
	 * @param PlayerAcceleration 플레이어 가속도
	 * @param DeltaTime 델타 타임
	 * @return 예측된 위치
	 */
	FVector CalculateAdvancedPrediction(const FVector& PlayerVelocity, const FVector& PlayerAcceleration, const float DeltaTime);

	/**
	 * @brief 최근 10프레임의 속도 데이터를 분석하여 움직임 패턴을 예측합니다.
	 * 
	 * @param PlayerVelocity 플레이어 속도
	 * @param PlayerSpeed 플레이어 속도 크기
	 * @return 패턴 기반 예측 위치
	 */
	FVector CalculatePatternBasedPrediction(const FVector& PlayerVelocity, float PlayerSpeed);

	/**
	 * @brief 보스와 플레이어 간의 거리에 따라 예측 시간을 조정합니다.
	 * 
	 * @return 거리 기반 예측 시간
	 */
	float CalculateDistanceBasedPredictionTime();

	/**
	 * @brief 플레이어의 이동 방향을 기반으로 회전을 예측하고 부드럽게 보간합니다.
	 * 
	 * @param PlayerVelocity 플레이어 속도
	 * @param PlayerSpeed 플레이어 속도 크기
	 * @return 예측된 회전
	 */
	FRotator CalculatePredictedRotation(const FVector& PlayerVelocity, float PlayerSpeed);

	// ===== 타겟 이동 방향 분석 함수들 =====
	
	/**
	 * @brief 타겟의 이동 방향을 계산하고 보스 기준으로 좌우 판단합니다.
	 */
	UFUNCTION(BlueprintCallable, Category="Target Direction")
	void CalculateTargetMovementDirection(FGameplayTag& OutDirectionState, const FBossTargetState& DirectionTags);

	// ===== 거리 상태 평가 함수들 =====
	
	/**
	 * @brief 거리에 따라 적절한 범위 태그를 반환합니다.
	 * 
	 * @param Distance 거리
	 * @return 거리 상태 태그
	 */
	UFUNCTION(BlueprintCallable, Category="Distance Evaluation")
	FGameplayTag EvaluateDistanceState(float Distance);

	/** 거리별 범위 기준값들 (에디터에서 각각 설정 가능) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Distance Settings", meta=(ClampMin="100.0", ClampMax="1000.0", ToolTip="거리별 범위 기준값들입니다.\n[0]: TooClose 범위 (300)\n[1]: Melee 범위 (400)\n[2]: Dash 범위 (1200) - 백스탭 거리와 연동\n[3]: Ranged 범위 (1600)\n[4]: OutOfRange 범위 (2000)"))
	TArray<float> DistanceThresholds = { 200.0f, 600.0f, 1600.0f, 2000.0f, 2000.0f };
	// ===== 거리 상태 태그들 =====
	
	/** 거리 상태 태그들 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Range Tags")
	FBossRangeTags RangeTags;

	/** 방향 상태 태그들 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Direction Tags")
	FBossTargetState DirectionTag;

private:
	/** 소유자 액터 */
	UPROPERTY()
	class APawn* Owner;
public:
	/** 타겟 액터 */
	UPROPERTY()
	class APawn* Target;

	/**
	 * @brief 플레이어를 찾는 함수
	 * 
	 * @return 찾은 플레이어 액터, 없으면 nullptr
	 */
	class APawn* FindPlayer();
};
