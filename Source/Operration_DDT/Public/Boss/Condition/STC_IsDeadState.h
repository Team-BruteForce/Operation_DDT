/**
 * @file STC_IsDeadState.h
 * @brief 사망 상태 체크 조건 헤더 파일
 * 
 * @details
 * StateTree에서 보스의 사망 상태를 체크하는 조건입니다.
 * 보스 상태 컴포넌트를 통해 사망 여부를 확인합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeConditionBlueprintBase.h"
#include "STC_IsDeadState.generated.h"

/**
 * @brief 사망 상태 체크 조건
 * 
 * StateTree에서 보스의 사망 상태를 체크하는 조건입니다.
 * 보스 상태 컴포넌트를 통해 사망 여부를 확인합니다.
 */
UCLASS()
class OPERRATION_DDT_API USTC_IsDeadState : public UStateTreeConditionBlueprintBase
{
	GENERATED_BODY()
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;
	
	
};
