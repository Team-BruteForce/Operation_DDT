/**
 * @file Task_Log.h
 * @brief 보스 로그 태스크 헤더 파일
 * 
 * @details
 * StateTree에서 디버깅 및 상태 모니터링을 위한 로그를 출력하는 태스크를 정의합니다.
 * 현재 활성화된 상태의 이름을 로그로 출력합니다.
 * 
 * @author [개발자명]
 * @date 2024-12-19
 */

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "Task_Log.generated.h"

/**
 * @brief 보스 로그 태스크
 * 
 * StateTree에서 디버깅 및 상태 모니터링을 위한 로그를 출력하는 태스크입니다.
 * 현재 활성화된 상태의 이름을 로그로 출력합니다.
 */
UCLASS()
class OPERRATION_DDT_API UTask_Log : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()
protected:
	/**
	 * @brief 상태 진입 시 호출되는 함수
	 * 
	 * @param Context StateTree 실행 컨텍스트
	 * @param Transition 상태 전환 결과
	 * @return EStateTreeRunStatus 상태 실행 상태
	 * 
	 * 상태에 진입할 때 부모 클래스의 기본 동작을 수행합니다.
	 */
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
	
	/**
	 * @brief 상태 실행 중 매 프레임 호출되는 함수
	 * 
	 * @param Context StateTree 실행 컨텍스트
	 * @param DeltaTime 델타 타임
	 * @return EStateTreeRunStatus 상태 실행 상태
	 * 
	 * 현재 활성화된 상태의 이름을 로그로 출력합니다.
	 */
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) override;

};
