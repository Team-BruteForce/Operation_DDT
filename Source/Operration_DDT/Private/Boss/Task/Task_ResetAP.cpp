// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Task/Task_ResetAP.h"
#include "Global.h"
#include "Boss/Component/CBossStatusComponent.h"

EStateTreeRunStatus UTask_ResetAP::EnterState(FStateTreeExecutionContext& Context,
                                              const FStateTreeTransitionResult& Transition)
{
	UCBossStatusComponent* Status=CHelpers::GetComponent<UCBossStatusComponent>(Boss);
	Status->ResetAp();
	return Super::EnterState(Context, Transition);
}
