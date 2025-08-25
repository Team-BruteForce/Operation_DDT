/**
 * @file Task_IncreaseAP.cpp
 * @brief 보스 AP 증가 태스크 구현 파일
 * 
 * @details
 * 보스의 액션 포인트를 증가시키는 태스크의 구현 파일입니다.
 * StateTree 기반 태스크 시스템을 사용하여 AP를 관리합니다.
 * 
 * @author [개발자명]
 * @date 2024-12-19
 */

#include "Boss/Task/Task_IncreaseAP.h"
#include "Global.h"
#include "Boss/Component/CBossStatusComponent.h"

/**
 * @brief 상태 진입 시 호출되는 함수
 * 
 * 보스의 액션 포인트를 증가시킵니다.
 */
EStateTreeRunStatus UTask_IncreaseAP::EnterState(FStateTreeExecutionContext& Context,
                                                 const FStateTreeTransitionResult& Transition)
{
	UCBossStatusComponent* Status=CHelpers::GetComponent<UCBossStatusComponent>(Boss);
	Status->IncreaseAP(AP);
	return EStateTreeRunStatus::Running;
}
