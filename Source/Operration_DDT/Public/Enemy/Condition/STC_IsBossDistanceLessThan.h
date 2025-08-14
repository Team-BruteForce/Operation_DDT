// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeConditionBlueprintBase.h"
#include "STC_IsBossDistanceLessThan.generated.h"

/**
 * @brief 보스와 플레이어 간 거리가 임계값보다 작은지 확인하는 조건 클래스
 * 
 * StateTree에서 사용되는 조건 클래스로, 보스와 플레이어 간의 거리가
 * 설정된 임계값보다 작거나 같은지 확인합니다.
 */
UCLASS()
class OPERRATION_DDT_API USTC_IsBossDistanceLessThan : public UStateTreeConditionBlueprintBase
{
	GENERATED_BODY()

	/**
	 * @brief 조건을 테스트하는 함수
	 * 
	 * @param Context StateTree 실행 컨텍스트
	 * @return true: 거리가 임계값 이하, false: 거리가 임계값보다 큼
	 */
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;
public:
	/**
	 * @brief 보스와 플레이어 간의 거리
	 * 
	 * 현재 보스와 플레이어 간의 거리를 저장합니다.
	 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Link")
	float BossToPlayerDistance=0.0f;
	
	/**
	 * @brief 거리 임계값
	 * 
	 * 비교할 거리의 임계값을 설정합니다.
	 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Link")
	float DistanceThreshold =200;
};
