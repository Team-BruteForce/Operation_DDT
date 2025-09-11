// Fill out your copyright notice in the Description page of Project Settings.


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
	return Super::EnterState(Context, Transition);
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
