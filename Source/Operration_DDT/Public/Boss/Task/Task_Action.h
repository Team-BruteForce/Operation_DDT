/**
 * @file Task_Action.h
 * @brief 보스 액션 태스크 헤더 파일
 * 
 * @details
 * 보스의 액션을 관리하는 태스크를 정의합니다.
 * StateTree 기반 태스크 시스템을 사용하여 액션 상태를 관리합니다.
 * 
 * @author [개발자명]
 * @date 2024-12-19
 */

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "Task_Action.generated.h"

/**
 * @brief 보스 액션 태스크
 * 
 * 보스의 액션을 관리하는 태스크입니다.
 * StateTree 기반 태스크 시스템을 사용하여 액션 상태를 관리합니다.
 */
UCLASS()
class OPERRATION_DDT_API UTask_Action : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()
protected:
	/**
	 * @brief 상태 진입 시 호출되는 함수
	 */
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
	
	/**
	 * @brief 액션 여부
	 * 
	 * 현재 액션을 실행 중인지 여부를 나타냅니다.
	 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Tag")
	bool IsAction;
};
