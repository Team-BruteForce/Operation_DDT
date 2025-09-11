// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeConditionBlueprintBase.h"
#include "STC_ProbabilityCheck.generated.h"

/**
 * 확률 조건 체크 컨디션
 * n% 확률로 전환
 */
UCLASS()
class OPERRATION_DDT_API USTC_ProbabilityCheck : public UStateTreeConditionBlueprintBase
{
	GENERATED_BODY()

protected:
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;

public:
	/**
	 * @brief 확률 (0.0 ~ 100.0)
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Probability Check", meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float Probability = 30.0f;
};
