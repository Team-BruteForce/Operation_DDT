/**
 * @file Task_ResetAP.cpp
 * @brief AP 리셋 태스크 구현 파일
 * 
 * @details
 * StateTree에서 보스의 AP(액션 포인트)를 리셋하는 태스크입니다.
 * 보스 상태 컴포넌트를 통해 AP를 초기화합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#include "Boss/Task/Task_ResetAP.h"
#include "Global.h"
#include "Boss/Component/CBossStatusComponent.h"

EStateTreeRunStatus UTask_ResetAP::EnterState(FStateTreeExecutionContext& Context,
                                              const FStateTreeTransitionResult& Transition)
{
	UCBossStatusComponent* Status=CHelpers::GetComponent<UCBossStatusComponent>(Boss);
	Status->ResetAp();
	return Super::EnterState(Context, Transition);
}
