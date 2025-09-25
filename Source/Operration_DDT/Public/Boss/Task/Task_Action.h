/**
 * @file Task_Action.h
 * @brief 보스 액션 태스크 헤더 파일
 * 
 * @section overview 개요
 * 이 파일은 보스의 액션을 관리하는 태스크를 정의합니다.
 * StateTree 기반 태스크 시스템을 사용하여 액션 상태를 관리합니다.
 * 
 * @section architecture 아키텍처
 * - UTask_Action: 보스 액션 관리 메인 태스크
 * - StateTree 태스크 시스템
 * - 액션 상태 관리 시스템
 * 
 * @section features 주요 기능
 * - 액션 상태 관리
 * - StateTree 연동
 * - 블루프린트 기반 태스크
 * - 액션 전환 처리
 * 
 * @section action_system 액션 시스템
 * - 액션 상태 관리
 * - StateTree 연동
 * - 액션 전환 처리
 * 
 * @author 이효원
 * @date 2024-12-19
 * @version 1.0
 */

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "Task_Action.generated.h"

/**
 * @brief 보스 액션 태스크 클래스
 * 
 * @details
 * 보스의 액션을 관리하는 태스크입니다.
 * StateTree 기반 태스크 시스템을 사용하여 액션 상태를 관리합니다.
 * 
 * @section design_patterns 설계 패턴
 * - 태스크 패턴: StateTree 태스크 시스템 활용
 * - 상태 패턴: 액션 상태 관리
 * - 전략 패턴: 다양한 액션 전략 지원
 * 
 * @section responsibilities 책임
 * - 액션 상태 관리
 * - StateTree 연동
 * - 액션 전환 처리
 * - 블루프린트 기반 태스크 실행
 * 
 * @section integration 연동 시스템
 * - StateTree: 태스크 실행 관리
 * - 보스 캐릭터: 액션 실행
 * - 액션 시스템: 액션 상태 관리
 * - 블루프린트: 태스크 구현
 * 
 * @section usage 사용법
 * 1. StateTree에 태스크 추가
 * 2. 액션 상태 설정
 * 3. 태스크 실행
 * 4. 액션 전환 처리
 */
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
