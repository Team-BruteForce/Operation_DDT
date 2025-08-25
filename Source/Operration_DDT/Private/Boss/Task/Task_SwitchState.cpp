/**
 * @file Task_SwitchState.cpp
 * @brief 보스 상태 전환 태스크 구현 파일
 * 
 * @details
 * 보스의 상태를 전환하는 태스크의 구현 파일입니다.
 * StateTree 기반 태스크 시스템을 사용하여 상태를 관리합니다.
 * 
 * @author [개발자명]
 * @date 2024-12-19
 */

#include "Boss/Task/Task_SwitchState.h"

#include "Boss/Component/BossStateComponent.h"
#include "Global.h"

/**
 * @brief 상태 진입 시 호출되는 함수
 * 
 * 보스의 상태를 전환합니다.
 */
EStateTreeRunStatus UTask_SwitchState::EnterState(FStateTreeExecutionContext& Context,
                                                  const FStateTreeTransitionResult& Transition)
{
	UBossStateComponent* State= CHelpers::GetComponent<UBossStateComponent>(Owner);
	State->SetStateTag(OwningStateTag);
	return EStateTreeRunStatus::Running;
}
