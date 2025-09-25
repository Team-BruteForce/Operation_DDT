/**
 * @file Task_TargetLocatonGap.cpp
 * @brief 타겟 위치 갭 태스크 구현 파일
 * 
 * @details
 * StateTree에서 보스의 타겟 위치 갭을 설정하는 태스크입니다.
 * 모션 워핑 컴포넌트와 타겟팅 컴포넌트를 사용합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#include "Boss/Task/Task_TargetLocatonGap.h"
#include "Global.h"
#include "MotionWarpingComponent.h"
#include "Boss/Component/CBossTargetingComponent.h"

EStateTreeRunStatus UTask_TargetLocatonGap::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	UMotionWarpingComponent* Motion = CHelpers::GetComponent<UMotionWarpingComponent>(Boss);
	UCBossTargetingComponent* TargetingComponent = CHelpers::GetComponent<UCBossTargetingComponent>(Boss);
	
	APawn* Target = TargetingComponent->FindPlayer();
	if (Target)
	{
		//보스와 타겟의 현재 위치
		FVector BossLocation = Boss->GetActorLocation();
		FVector TargetLocation = Target->GetActorLocation();
		
		// 보스에서 타겟으로의 방향 벡터
		FVector DirectionToTarget = (TargetLocation - BossLocation).GetSafeNormal();
		
		// KeepDistance만큼 떨어진 위치 계산 (타겟에서 보스 방향으로 KeepDistance만큼)
		FVector DesiredLocation = TargetLocation - (DirectionToTarget * KeepDistance);
		
		Motion->AddOrUpdateWarpTargetFromLocation(TargetName, DesiredLocation);
	}
	
	return Super::Tick(Context, DeltaTime);
}
