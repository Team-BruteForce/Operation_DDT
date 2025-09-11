// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Task/Task_SideMoveGetLocation.h"
#include "Global.h"
#include "Boss/Component/FlyingComponent.h"

EStateTreeRunStatus UTask_SideMoveGetLocation::EnterState(FStateTreeExecutionContext& Context,
                                                          const FStateTreeTransitionResult& Transition)
{
	UFlyingComponent* FlyingComponent=CHelpers::GetComponent<UFlyingComponent>(Boss);
	FlyingComponent->StartSplineSemicircleMovement();

	return Super::EnterState(Context, Transition);
}
