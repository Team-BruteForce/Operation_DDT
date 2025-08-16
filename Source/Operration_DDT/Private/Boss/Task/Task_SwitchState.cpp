// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Task/Task_SwitchState.h"

#include "Boss/Component/BossStateComponent.h"
#include "Global.h"

EStateTreeRunStatus UTask_SwitchState::EnterState(FStateTreeExecutionContext& Context,
                                                  const FStateTreeTransitionResult& Transition)
{
	UBossStateComponent* State= CHelpers::GetComponent<UBossStateComponent>(Owner);
	State->SetStateTag(OwningStateTag);
	return Super::EnterState(Context, Transition);
}
