/**
 * @file Task_Hovering.cpp
 * @brief 호버링 태스크 구현 파일
 * 
 * @details
 * StateTree에서 보스가 공중에서 호버링하는 태스크입니다.
 * 비행 컴포넌트를 통해 호버링 상태를 시작하고 종료합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#include "Boss/Task/Task_Hovering.h"

#include "Global.h"
#include "Boss/Component/FlyingComponent.h"

EStateTreeRunStatus UTask_Hovering::EnterState(FStateTreeExecutionContext& Context,
                                               const FStateTreeTransitionResult& Transition)
{
	UFlyingComponent* FlyingComponent=CHelpers::GetComponent<UFlyingComponent>(Boss);
	if (!FlyingComponent)	return EStateTreeRunStatus::Failed;
	FlyingComponent->StartHovering();
	return EStateTreeRunStatus::Running;
}

void UTask_Hovering::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	UFlyingComponent* FlyingComponent=CHelpers::GetComponent<UFlyingComponent>(Boss);
	CheckNull(FlyingComponent);
	FlyingComponent->StopHovering();
	Super::ExitState(Context, Transition);
}
