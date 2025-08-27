// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Task/Task_MotionWarpTargetSetting.h"
#include "Global.h"
#include "MotionWarpingComponent.h"

EStateTreeRunStatus UTask_MotionWarpTargetSetting::EnterState(FStateTreeExecutionContext& Context,
                                                              const FStateTreeTransitionResult& Transition)
{
	UMotionWarpingComponent* Motion= CHelpers::GetComponent<UMotionWarpingComponent>(Boss);
	Motion->AddOrUpdateWarpTargetFromLocation( TargetName,TargetLocation);
	return EStateTreeRunStatus::Running;
}
