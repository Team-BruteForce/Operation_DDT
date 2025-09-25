/**
 * @file Task_FlySetLocation.cpp
 * @brief 비행 위치 설정 태스크 구현 파일
 * 
 * @details
 * StateTree에서 보스의 비행 위치를 설정하는 태스크입니다.
 * 스플라인 거리 유지를 통해 특정 위치로 이동합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#include "Boss/Task/Task_FlySetLocation.h"

#include "Boss/Component/FlyingComponent.h"
#include "Global.h"
#include "StateTreeExecutionContext.h"

EStateTreeRunStatus UTask_FlySetLocation::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	if(bStarted) return EStateTreeRunStatus::Running;
	bStarted=true;
	UFlyingComponent* FlyingComponent=CHelpers::GetComponent<UFlyingComponent>(Boss);
	FlyingComponent->StartSplineDistanceMaintenance(1500.0f);
	return EStateTreeRunStatus::Running;
}

void UTask_FlySetLocation::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
}