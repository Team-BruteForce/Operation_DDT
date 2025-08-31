/**
 * @file Task_RotateTowardsPlayer.cpp
 * @brief 보스 플레이어 방향 회전 태스크 구현 파일
 * 
 * @details
 * 보스가 플레이어 방향으로 회전하는 태스크의 구현 파일입니다.
 * StateTree에서 사용되는 태스크로, 보스가 플레이어를 향해 부드럽게 회전하는 기능을 제공합니다.
 * 
 * @author [개발자명]
 * @date 2024-12-19
 */

#include "Boss/Task/Task_RotateTowardsPlayer.h"
#include "Global.h"
#include "Boss/Component/CBossMovementComponent.h"

/**
 * @brief 매 프레임 호출되는 Tick 함수
 * 
 * @param Context StateTree 실행 컨텍스트
 * @param DeltaTime 델타 타임
 * @return EStateTreeRunStatus::Running 상태가 계속 실행됨을 나타냄
 * 
 * 보스가 플레이어를 향해 회전합니다.
 */
EStateTreeRunStatus UTask_RotateTowardsPlayer::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	// 로그로 Tick 호출 확인
	UCBossMovementComponent* Movement= CHelpers::GetComponent<UCBossMovementComponent>(Owner);
	Movement->RotateTowardsPlayer(DeltaTime,RotationSpeed);
	return EStateTreeRunStatus::Running;
}
