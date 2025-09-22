// Fill out your copyright notice in the Description page of Project Settings.


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
