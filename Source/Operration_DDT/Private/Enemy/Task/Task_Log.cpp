// Fill out your copyright notice in the Description page of Project Settings.

/**
 * @file Task_Log.cpp
 * @brief StateTree 로그 태스크 구현 파일
 * 
 * 이 파일은 StateTree에서 디버깅 및 상태 모니터링을 위한 로그 태스크를 구현합니다.
 * 현재 활성화된 상태의 이름을 로그로 출력하여 StateTree의 동작을 추적할 수 있습니다.
 * 
 * 주요 기능:
 * - 현재 활성 상태 이름 로그 출력
 * - StateTree 디버깅 지원
 * - 상태 모니터링 및 추적
 */

#include "Enemy/Task/Task_Log.h"

#include "StateTreeExecutionContext.h"
#include "Utilities/CLog.h"

/**
 * @brief 상태 진입 시 호출되는 함수
 * 
 * @param Context StateTree 실행 컨텍스트
 * @param Transition 상태 전환 결과
 * @return EStateTreeRunStatus 부모 클래스의 EnterState 결과
 * 
 * 상태에 진입할 때 부모 클래스의 기본 동작을 수행합니다.
 * 현재는 로그 출력을 위한 상태이므로 특별한 초기화 작업은 수행하지 않습니다.
 */
EStateTreeRunStatus UTask_Log::EnterState(FStateTreeExecutionContext& Context,
                                          const FStateTreeTransitionResult& Transition)
{
	return Super::EnterState(Context, Transition);
}

/**
 * @brief 상태 실행 중 매 프레임 호출되는 함수
 * 
 * @param Context StateTree 실행 컨텍스트
 * @param DeltaTime 델타 타임
 * @return EStateTreeRunStatus 부모 클래스의 Tick 결과
 * 
 * 현재 활성화된 상태의 이름을 로그로 출력합니다.
 * 디버깅 및 상태 모니터링을 위한 용도로 사용됩니다.
 */
EStateTreeRunStatus UTask_Log::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	CLog::Print(Context.GetActiveStateName());
	return Super::Tick(Context, DeltaTime);
}
