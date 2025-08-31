// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Task/Task_TargetLocationFeet.h"
#include "Global.h"
#include "MotionWarpingComponent.h"
#include "Boss/Component/CBossTargetingComponent.h"

EStateTreeRunStatus UTask_TargetLocationFeet::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	UMotionWarpingComponent* Motion= CHelpers::GetComponent<UMotionWarpingComponent>(Boss);
	UCBossTargetingComponent* TargetingComponent= CHelpers::GetComponent<UCBossTargetingComponent>(Boss);
	Motion->AddOrUpdateWarpTargetFromLocation( TargetName,TargetingComponent->FindPlayer()->GetActorLocation());
	return Super::Tick(Context, DeltaTime);
}
