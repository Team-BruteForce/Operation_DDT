/**
 * @file Task_TargetLocationFeet.cpp
 * @brief 타겟 위치 발 태스크 구현 파일
 * 
 * @details
 * StateTree에서 보스의 타겟 위치 발을 설정하는 태스크입니다.
 * 모션 워핑 컴포넌트와 타겟팅 컴포넌트를 사용합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#include "Boss/Task/Task_TargetLocationFeet.h"
#include "Global.h"
#include "MotionWarpingComponent.h"
#include "Boss/Component/CBossTargetingComponent.h"

EStateTreeRunStatus UTask_TargetLocationFeet::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	UMotionWarpingComponent* Motion= CHelpers::GetComponent<UMotionWarpingComponent>(Boss);
	UCBossTargetingComponent* TargetingComponent= CHelpers::GetComponent<UCBossTargetingComponent>(Boss);
	TargetLocation=TargetingComponent->FindPlayer()->GetActorLocation();
	Motion->AddOrUpdateWarpTargetFromLocation( TargetName,FVector(TargetLocation.X,TargetLocation.Y,0));
	return Super::Tick(Context, DeltaTime);
}
