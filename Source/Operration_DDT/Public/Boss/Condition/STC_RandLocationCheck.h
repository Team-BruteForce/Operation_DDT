// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeConditionBlueprintBase.h"
#include "STC_RandLocationCheck.generated.h"

/**
 * 랜덤 위치 이동 상태 체크 컨디션
 * 랜덤 이동 중/완료 체크
 */
UCLASS()
class OPERRATION_DDT_API USTC_RandLocationCheck : public UStateTreeConditionBlueprintBase
{
	GENERATED_BODY()

protected:
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;

public:
	/**
	 * @brief 보스 액터 (StateTree에서 설정)
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rand Location Check")
	class APawn* Boss = nullptr;
};
