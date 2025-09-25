/**
 * @file Task_SetVectorTargetLocation.cpp
 * @brief 벡터 타겟 위치 설정 태스크 구현 파일
 * 
 * @details
 * StateTree에서 보스의 벡터 타겟 위치를 설정하는 태스크입니다.
 * 모션 워핑 컴포넌트와 타겟팅 컴포넌트를 사용합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

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
