/**
 * @file Task_SetCurrentAction.cpp
 * @brief 현재 액션 설정 태스크 구현 파일
 * 
 * @details
 * StateTree에서 보스의 현재 액션을 설정하는 태스크입니다.
 * 보스 상태 컴포넌트에 마지막 활성 태그를 저장합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#include "Boss/Task/Task_SetCurrentAction.h"

#include "Global.h"
#include "Boss/Component/BossStateComponent.h"

/**
 * @brief 상태 진입 시 호출되는 함수
 * 
 * @param Context StateTree 실행 컨텍스트
 * @param Transition 상태 전환 결과
 * @return EStateTreeRunStatus 실행 상태
 * 
 * 보스 상태 컴포넌트에 선택된 액션 태그를 저장합니다.
 */
EStateTreeRunStatus UTask_SetCurrentAction::EnterState(FStateTreeExecutionContext& Context,
                                                       const FStateTreeTransitionResult& Transition)
{
	UBossStateComponent* State=CHelpers::GetComponent<UBossStateComponent>(Boss);
	if (State)
		State->LastActiveTag=SelectActionTag;
	return Super::EnterState(Context, Transition);
}
