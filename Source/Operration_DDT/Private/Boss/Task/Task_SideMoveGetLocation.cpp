/**
 * @file Task_SideMoveGetLocation.cpp
 * @brief 사이드 이동 위치 획득 태스크 구현 파일
 * 
 * @details
 * StateTree에서 보스의 사이드 이동 위치를 획득하는 태스크입니다.
 * 비행 컴포넌트를 통해 사이드 이동 위치를 계산합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#include "Boss/Task/Task_SideMoveGetLocation.h"
#include "Global.h"
#include "Boss/Component/FlyingComponent.h"

EStateTreeRunStatus UTask_SideMoveGetLocation::EnterState(FStateTreeExecutionContext& Context,
                                                          const FStateTreeTransitionResult& Transition)
{
	UFlyingComponent* FlyingComponent=CHelpers::GetComponent<UFlyingComponent>(Boss);
	FlyingComponent->StartDistanceMaintenance();

	// 이동을 시작하고 Running 상태 유지 (이벤트로 전환됨)
	return EStateTreeRunStatus::Running;
}
