// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Task/Task_SetCurrentAction.h"

#include "Global.h"
#include "Boss/Component/BossStateComponent.h"

EStateTreeRunStatus UTask_SetCurrentAction::EnterState(FStateTreeExecutionContext& Context,
                                                       const FStateTreeTransitionResult& Transition)
{
	UBossStateComponent* State=CHelpers::GetComponent<UBossStateComponent>(Boss);
	if (State)
		State->LastActiveTag=SelectActionTag;
	return Super::EnterState(Context, Transition);
}
