// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeConditionBlueprintBase.h"
#include "STC_TimerCheck.generated.h"

/**
 * 시간 조건 체크 컨디션
 * n~m초 사이 랜덤 시간 체크
 */
UCLASS()
class OPERRATION_DDT_API USTC_TimerCheck : public UStateTreeConditionBlueprintBase
{
	GENERATED_BODY()

protected:
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;

private:
	/**
	 * @brief 타이머 초기화
	 */
	void ResetTimer();

public:
	/**
	 * @brief 최소 시간 (초)
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer Check")
	float MinTime = 5.0f;

	/**
	 * @brief 최대 시간 (초)
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer Check")
	float MaxTime = 10.0f;

	/**
	 * @brief 현재 타이머
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Timer Check")
	mutable float CurrentTimer = 0.0f;

	/**
	 * @brief 목표 시간
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Timer Check")
	mutable float TargetTime = 0.0f;

};
