// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeConditionBlueprintBase.h"
#include "STC_DistanceCheck.generated.h"

/**
 * 거리 조건 체크 컨디션
 * 플레이어가 너무 가까움 OR 너무 멀음 체크
 */
UCLASS()
class OPERRATION_DDT_API USTC_DistanceCheck : public UStateTreeConditionBlueprintBase
{
	GENERATED_BODY()

protected:
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;



public:
	/**
	 * @brief 최소 거리 (이보다 가까우면 너무 가까움)
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Distance Check")
	float MinDistance = 500.0f;
	/**
	 * @brief 최대 거리 (이보다 멀면 너무 멀음)
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Distance Check")
	float MaxDistance = 4000.0f;
	/**
 * @brief 현재 거리 (이보다 멀면 너무 멀음)
 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Distance Check")
	float Current_Dist=0.0f;
};
