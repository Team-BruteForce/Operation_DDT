// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Task/Task_SetCurrentAction.h"

EStateTreeRunStatus UTask_SetCurrentAction::EnterState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition)
{
	CurrentActionTag=SelectActionTag;
	return Super::EnterState(Context, Transition);
}
