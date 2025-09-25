/**
 * @file Task_FlyKeepingDistance.cpp
 * @brief 비행 거리 유지 태스크 구현 파일
 * 
 * @details
 * StateTree에서 보스가 비행하면서 거리를 유지하는 태스크입니다.
 * 스플라인 순차 이동을 통해 플레이어와의 거리를 조절합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#include "Boss/Task/Task_FlyKeepingDistance.h"
#include "Boss/Component/FlyingComponent.h"
#include "Global.h"
#include "StateTreeExecutionContext.h"

EStateTreeRunStatus UTask_FlyKeepingDistance::EnterState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition)
{
	// FlyingComponent 가져오기
	UFlyingComponent* FlyingComponent=CHelpers::GetComponent<UFlyingComponent>(Boss);

	// 스플라인 순차 이동 시작
	FlyingComponent->StartSplineOrbit();
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus UTask_FlyKeepingDistance::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	// 성공적으로 완료
	return EStateTreeRunStatus::Running;
}

void UTask_FlyKeepingDistance::ExitState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition)
{
	Super::ExitState(Context, Transition);
	UFlyingComponent* FlyingComponent=CHelpers::GetComponent<UFlyingComponent>(Boss);
	
	FlyingComponent->StartSplineOrbit();
	
}
