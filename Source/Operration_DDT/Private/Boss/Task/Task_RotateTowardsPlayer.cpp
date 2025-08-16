// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Task/Task_RotateTowardsPlayer.h"
#include "Global.h"
#include "Boss/Component/CBossMovementComponent.h"

EStateTreeRunStatus UTask_RotateTowardsPlayer::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	// 로그로 Tick 호출 확인
	CLog::Log("UTask_RotateTowardsPlayer::Tick");
	
	UCBossMovementComponent* Movement= CHelpers::GetComponent<UCBossMovementComponent>(Owner);
	Movement->RotateTowardsPlayer(DeltaTime,1.5f);
	return EStateTreeRunStatus::Running;  // 직접 Running 반환
}
