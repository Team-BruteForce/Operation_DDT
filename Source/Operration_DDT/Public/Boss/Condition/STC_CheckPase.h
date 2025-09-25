/**
 * @file STC_CheckPase.h
 * @brief 페이즈 체크 조건 헤더 파일
 * 
 * @details
 * StateTree에서 보스의 페이즈를 체크하는 조건입니다.
 * 보스의 현재 페이즈 상태를 확인합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeConditionBlueprintBase.h"
#include "STC_CheckPase.generated.h"

/**
 * @brief 페이즈 체크 조건
 * 
 * StateTree에서 보스의 페이즈를 체크하는 조건입니다.
 * 보스의 현재 페이즈 상태를 확인합니다.
 */
UCLASS()
class OPERRATION_DDT_API USTC_CheckPase : public UStateTreeConditionBlueprintBase
{
	GENERATED_BODY()
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conditions")
	class AActor* Owner;
};
