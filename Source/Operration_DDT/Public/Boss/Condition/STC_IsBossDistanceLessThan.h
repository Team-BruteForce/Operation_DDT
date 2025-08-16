// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeConditionBlueprintBase.h"
#include "Boss/StateTree/CBossEnemyStateTreeEvaluator.h"
#include "STC_IsBossDistanceLessThan.generated.h"

/**
 * @brief 보스가 너무 가까운 거리에 있는지 확인하는 조건 클래스
 * 
 * StateTree에서 사용되는 조건 클래스로, Evaluator에서 설정된
 * CurrentRangeTag가 TooClose 태그와 일치하는지 확인합니다.
 */
UCLASS()
class OPERRATION_DDT_API USTC_IsBossDistanceLessThan : public UStateTreeConditionBlueprintBase
{
	GENERATED_BODY()
	
	/**
	 * @brief 조건을 테스트하는 함수
	 * 
	 * @param Context StateTree 실행 컨텍스트
	 * @return true: 너무 가까운 거리, false: 적당한 거리
	 * 
	 * Evaluator에서 설정된 CurrentRangeTag가 TooClose 태그와 일치하는지 확인합니다.
	 */
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;
	
	// Evaluator에서 직접 바인딩할 변수들
	UPROPERTY(EditAnywhere, Category="Range Tags")
	FBossRangeTags RangeTags;
	
	UPROPERTY(EditAnywhere, Category="Current Tag")
	FGameplayTag CurrentRangeTag;
};
