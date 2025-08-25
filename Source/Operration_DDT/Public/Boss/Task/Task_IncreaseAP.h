/**
 * @file Task_IncreaseAP.h
 * @brief 보스 AP 증가 태스크 헤더 파일
 * 
 * @details
 * 보스의 액션 포인트를 증가시키는 태스크를 정의합니다.
 * StateTree 기반 태스크 시스템을 사용하여 AP를 관리합니다.
 * 
 * @author [개발자명]
 * @date 2024-12-19
 */

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "Task_IncreaseAP.generated.h"

/**
 * @brief 보스 AP 증가 태스크
 * 
 * 보스의 액션 포인트를 증가시키는 태스크입니다.
 * StateTree 기반 태스크 시스템을 사용하여 AP를 관리합니다.
 */
UCLASS()
class OPERRATION_DDT_API UTask_IncreaseAP : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()
	

	/**
	 * @brief 상태 진입 시 호출되는 함수
	 */
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
	
	/**
	 * @brief 보스 Pawn 참조
	 */
	UPROPERTY(EditAnywhere)
	class APawn* Boss;

	/**
	 * @brief 증가할 AP 양
	 */
	UPROPERTY(EditAnywhere)
	float AP;
};
