// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Task/Task_SideMoveGetLocation.h"
#include "Global.h"
#include "Boss/Component/FlyingComponent.h"

EStateTreeRunStatus UTask_SideMoveGetLocation::EnterState(FStateTreeExecutionContext& Context,
                                                          const FStateTreeTransitionResult& Transition)
{
	UFlyingComponent* FlyingComponent=CHelpers::GetComponent<UFlyingComponent>(Boss);
	FlyingComponent->MoveSideways();

	// 이동을 시작하고 Running 상태 유지 (이벤트로 전환됨)
	return EStateTreeRunStatus::Running;
}
