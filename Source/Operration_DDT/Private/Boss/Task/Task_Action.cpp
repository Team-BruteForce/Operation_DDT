/**
 * @file Task_Action.cpp
 * @brief 보스 액션 태스크 구현 파일
 * 
 * @details
 * 보스의 액션을 관리하는 태스크의 구현 파일입니다.
 * StateTree 기반 태스크 시스템을 사용하여 액션 상태를 관리합니다.
 * 
 * @author [개발자명]
 * @date 2024-12-19
 */

#include "Boss/Task/Task_Action.h"

#include "StateTreeExecutionContext.h"
#include "Boss/StateTree/CBossEnemyStateTreeEvaluator.h"
#include "Utilities/CLog.h"

/**
 * @brief 상태 진입 시 호출되는 함수
 * 
 * 액션 상태를 설정합니다.
 */
EStateTreeRunStatus UTask_Action::EnterState(FStateTreeExecutionContext& Context,
                                             const FStateTreeTransitionResult& Transition)
{
	CLog::Log("UTask_Action");
	IsAction=true;
	return Super::EnterState(Context, Transition);
}
