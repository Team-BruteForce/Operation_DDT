// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Task/Task_SetVectorTargetLocation.h"
#include "Global.h"
#include "MotionWarpingComponent.h"
#include "Boss/Component/CBossTargetingComponent.h"

EStateTreeRunStatus UTask_SetVectorTargetLocation::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	UMotionWarpingComponent* Motion= CHelpers::GetComponent<UMotionWarpingComponent>(Boss);
	UCBossTargetingComponent* TargetingComponent= CHelpers::GetComponent<UCBossTargetingComponent>(Boss);
	Motion->AddOrUpdateWarpTargetFromLocation( TargetName,TargetLocation);
	return Super::Tick(Context, DeltaTime);
}
